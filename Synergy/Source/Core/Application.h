/// \ project: Synergy-RayTracing
/// \ file: Application.h
/// \ author: Thomas Clermont

#pragma once
#include "Core.h"
#include "Window.h"
#include "Layer.h"

#include "Rendering/Messenger.h"
#include "Rendering/DescriptorPool.h"
#include "Rendering/Framebuffers.h"
#include "Rendering/CommandPool.h"
#include "Rendering/Sync.h"
#include "Editor/Editor.h"


namespace SNG
{

	struct AppInfo
	{
		std::string Name;
		int Width;
		int Height;
	};


	class Application
	{
	public:

		Application(const AppInfo& infos);
		~Application();

		void Run();
		void SetMenuBarCallback(std::function<void()> func) { m_Editor.SetMenuBarCallback(func); }

		static VkInstance GetInstance();
		static VkPhysicalDevice GetPhysicalDevice();
		static uint32_t GetQueueFamily();
		static VkDevice GetDevice();
		static VkQueue GetQueue();
		static VkSurfaceKHR GetSurface();
		static VkDescriptorPool GetDescriptorPool();
		static VkRenderPass GetRenderPass();
		static std::vector<VkFramebuffer> GetFramebuffers();
		static VkCommandPool GetCommandPool();
		static VkExtent2D GetImageExtent();

		VkCommandBuffer CreateCommandBuffer();
		void FlushCommandBuffers(VkCommandBuffer commandBuffer);
		void AddClearFunction(std::function<void()>&& func);

		inline static Application* GetApplication() { return s_Application; }
		inline Window* GetWindow() const { return m_Window.get(); }

		template<typename T>
		void PushLayer()
		{
			if (!std::is_base_of<Layer, T>::value) __debugbreak();
			m_Layers.emplace_back(std::make_shared<T>())->OnAttach();
		}

	private:

		void OnRender();
		void OnUpdate();
		void Draw();

	private:

		static Application* s_Application;

		// Rendering
		Scope<Window> m_Window;
		Ref<Instance> r_Instance;
		Ref<Messenger> r_Messenger;
		Ref<PhysicalDevice> r_PhysicalDevice;
		Ref<Device> r_Device;
		Ref<Surface> r_Surface;
		Ref<DescriptorPool> r_DescriptorPool;
		Ref<Swapchain> r_Swapchain;
		Ref<RenderPass> r_RenderPass;
		Ref<Framebuffers> r_Framebuffers;
		Ref<CommandPool> r_CommandPool;
		Ref<Sync> r_Sync;
		uint32_t m_CurrentFrame = 0;

		Editor m_Editor;
		std::vector<std::shared_ptr<Layer>> m_Layers;
		std::vector<VkCommandBuffer> m_SubmitCommands;
		std::vector<std::vector<std::function<void()>>> m_ClearFunctions;

		friend void OnWindowResizeFunc(GLFWwindow* window, int width, int height);
		friend void RecreateSwapchain();
	};


	// TO BE DEFINED IN CLIENT
	Application* CreateApplication();

}