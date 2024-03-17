/// \ project: Synergy-RayTracing
/// \ file: Editor.cpp
/// \ author: Thomas Clermont

#include "Editor.h"
#include "Core/Application.h"


namespace SNG
{

	void Editor::Init()
	{
		auto window = Application::GetApplication()->GetWindow();
		m_MenuBarCallback = []() {};

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Ubuntu-Bold.ttf", 18);

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 100.0f;
		style.WindowRounding = 5.0f;
		style.ItemSpacing = ImVec2(10, 10);

		ImGui_ImplGlfw_InitForVulkan(window->Raw(), true);

		ImGui_ImplVulkan_InitInfo initInfo{};
		initInfo.Instance = Application::GetInstance();
		initInfo.Device = Application::GetDevice();
		initInfo.PhysicalDevice = Application::GetPhysicalDevice();
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		initInfo.MinImageCount = MAX_IMAGE_COUNT;
		initInfo.ImageCount = MAX_IMAGE_COUNT;
		initInfo.DescriptorPool = Application::GetDescriptorPool();
		initInfo.Queue = Application::GetQueue();
		ImGui_ImplVulkan_Init(&initInfo, Application::GetRenderPass());

		auto commandBuffer = OneTimeCommandBuffer();
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		EndOneTimeCommandBuffer(commandBuffer);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	Editor::~Editor()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::RecordCommandBuffer(uint32_t index)
	{
		auto commandBuffer = Application::GetApplication()->CreateCommandBuffer();

		VkClearValue clearValues[1] = {};
		clearValues[0] = { { 0.1f, 0.1f, 0.1f, 1.0f } };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = Application::GetRenderPass();
		renderPassInfo.framebuffer = Application::GetFramebuffers()[index];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = Application::GetImageExtent();
		renderPassInfo.clearValueCount = (uint32_t)IM_ARRAYSIZE(clearValues);
		renderPassInfo.pClearValues = clearValues;

		ImGui::End();
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

		vkCmdEndRenderPass(commandBuffer);
		vkEndCommandBuffer(commandBuffer);

		Application::GetApplication()->FlushCommandBuffers(commandBuffer);
	}

	void Editor::DrawFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static bool dockspace_open = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				window_flags |= ImGuiWindowFlags_NoBackground;
			}
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Editor dockspace", &dockspace_open, window_flags);
			if (!opt_padding) ImGui::PopStyleVar();
			if (opt_fullscreen) ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				m_MenuBarCallback();
				ImGui::EndMenuBar();
			}
		}
	}

}
