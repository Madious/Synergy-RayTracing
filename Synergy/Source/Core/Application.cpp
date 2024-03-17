/// \ project: Synergy-RayTracing
/// \ file: Application.cpp
/// \ author: Thomas Clermont

#include "Application.h"


namespace SNG
{

	Application* Application::s_Application = nullptr;


	VkInstance Application::GetInstance() { return s_Application->r_Instance->Raw(); }
	VkPhysicalDevice Application::GetPhysicalDevice() { return s_Application->r_PhysicalDevice->Raw(); }
	uint32_t Application::GetQueueFamily() { return s_Application->r_Device->GetQueueFamily(); }
	VkDevice Application::GetDevice() { return s_Application->r_Device->Raw(); }
	VkQueue Application::GetQueue() { return s_Application->r_Device->GetQueue(); }
	VkSurfaceKHR Application::GetSurface() { return s_Application->r_Surface->Raw(); }
	VkDescriptorPool Application::GetDescriptorPool() { return s_Application->r_DescriptorPool->Raw(); }
	VkRenderPass Application::GetRenderPass() { return s_Application->r_RenderPass->Raw(); }
	std::vector<VkFramebuffer> Application::GetFramebuffers() { return s_Application->r_Framebuffers->Raw(); }
	VkCommandPool Application::GetCommandPool() { return s_Application->r_CommandPool->Raw(); }
	VkExtent2D Application::GetImageExtent() { return s_Application->r_Swapchain->GetImageExtent(); }




	void RecreateSwapchain()
	{
		auto app = Application::GetApplication();
		app->r_Framebuffers = nullptr;
		app->r_RenderPass = nullptr;
		app->r_Swapchain = nullptr;
		app->r_Swapchain = MakeRef<Swapchain>(app->m_Window.get(), app->r_PhysicalDevice, app->r_Device, app->r_Surface);
		app->r_RenderPass = MakeRef<RenderPass>(app->r_Device);
		app->r_Framebuffers = MakeRef<Framebuffers>(app->r_Device, app->r_Swapchain, app->r_RenderPass);
	}

	void OnWindowResizeFunc(GLFWwindow* window, int width, int height)
	{
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(Application::GetApplication()->GetWindow()->Raw(), &width, &height);
			glfwWaitEvents();
		}

		Application::GetApplication()->AddClearFunction([device = Application::GetDevice()]() {
				vkDeviceWaitIdle(device);
				RecreateSwapchain();
			});
	}




	Application::Application(const AppInfo& infos)
	{
		s_Application = this;
		m_Window = MakeScope<Window>(infos.Width, infos.Height, infos.Name);
		m_Window->SetFramebufferSizeCallback(OnWindowResizeFunc);

		// Rendering
		{
			r_Instance = MakeRef<Instance>();
			r_Messenger = MakeRef<Messenger>(r_Instance);
			r_PhysicalDevice = MakeRef<PhysicalDevice>(r_Instance);
			r_Device = MakeRef<Device>(r_PhysicalDevice);
			r_Surface = MakeRef<Surface>(m_Window.get(), r_Instance);
			r_Swapchain = MakeRef<Swapchain>(m_Window.get(), r_PhysicalDevice, r_Device, r_Surface);
			r_RenderPass = MakeRef<RenderPass>(r_Device);
			r_Framebuffers = MakeRef<Framebuffers>(r_Device, r_Swapchain, r_RenderPass);
			r_DescriptorPool = MakeRef<DescriptorPool>(r_Device, 1000);
			r_CommandPool = MakeRef<CommandPool>(r_Device);
			r_Sync = MakeRef<Sync>(r_Device);
		}

		m_Editor.Init();
		m_ClearFunctions.resize(r_Swapchain->GetImageCount());
	}

	Application::~Application()
	{
		m_Layers.clear();

		for (auto& frame : m_ClearFunctions)
			for (auto& func : frame)
				func();
		m_ClearFunctions.clear();
	}

	void Application::Run()
	{
		while (!m_Window->IsClose())
		{
			m_Window->OnUpdate();
			OnUpdate();
			Draw();
		}

		vkDeviceWaitIdle(Application::GetDevice());
	}

	void Application::OnUpdate()
	{
		static float time = 0.0f;
		float elapsedTime = glfwGetTime();
		float ts = elapsedTime - time;
		time = elapsedTime;

		m_SubmitCommands.clear();
		for (auto& func : m_ClearFunctions[m_CurrentFrame])
			func();
		m_ClearFunctions[m_CurrentFrame].clear();

		for (auto& layer : m_Layers)
			layer->OnUpdate(ts);
	}

	void Application::OnRender()
	{
		for (auto& layer : m_Layers)
			layer->OnRender();
	}

	void Application::AddClearFunction(std::function<void()>&& func)
	{
		m_ClearFunctions[m_CurrentFrame].push_back(func);
	}

	void Application::Draw()
	{
		vkWaitForFences(Application::GetDevice(), 1, &r_Sync->InFlight()[m_CurrentFrame], VK_TRUE, UINT64_MAX);
		uint32_t imageIndex;
		vkAcquireNextImageKHR(Application::GetDevice(), r_Swapchain->Raw(), INT64_MAX,
			r_Sync->ImageAvaible()[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);


		m_Editor.DrawFrame();
		OnRender();
		m_Editor.RecordCommandBuffer(imageIndex);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		if (r_Sync->ImageInFlight()[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(Application::GetDevice(), 1, &r_Sync->ImageInFlight()[imageIndex], VK_TRUE, UINT64_MAX);
		r_Sync->ImageInFlight()[imageIndex] = r_Sync->ImageInFlight()[m_CurrentFrame];

		// SUBMIT
		{
			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = (uint32_t)m_SubmitCommands.size();
			submitInfo.pCommandBuffers = m_SubmitCommands.data();
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &r_Sync->ImageAvaible()[m_CurrentFrame];
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &r_Sync->RenderFinished()[m_CurrentFrame];

			vkResetFences(Application::GetDevice(), 1, &r_Sync->InFlight()[m_CurrentFrame]);
			auto err = vkQueueSubmit(Application::GetQueue(), 1, &submitInfo, r_Sync->InFlight()[m_CurrentFrame]);
			check_vk_result(err);
		}

		// PRESENT
		{
			VkSwapchainKHR swapchains[1] = { r_Swapchain->Raw() };

			VkPresentInfoKHR presentInfo{};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapchains;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &r_Sync->RenderFinished()[m_CurrentFrame];
			presentInfo.pImageIndices = &imageIndex;
			auto err = vkQueuePresentKHR(Application::GetQueue(), &presentInfo);
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_IMAGE_COUNT;
	}

	VkCommandBuffer Application::CreateCommandBuffer()
	{
		VkCommandBuffer commandBuffer;

		// Allocate command buffer
		{
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = r_CommandPool->Raw();
			allocInfo.commandBufferCount = 1;
			auto err = vkAllocateCommandBuffers(Application::GetDevice(), &allocInfo, &commandBuffer);
			check_vk_result(err, "Erreur lors de la creation d'un command buffer");
		}

		// Begin command buffer
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			auto err = vkBeginCommandBuffer(commandBuffer, &beginInfo);
			check_vk_result(err, "Erreur lors du commencement d'un command buffer");
		}

		return commandBuffer;
	}

	void Application::FlushCommandBuffers(VkCommandBuffer commandBuffer)
	{
		m_SubmitCommands.push_back(commandBuffer);
	}

}

