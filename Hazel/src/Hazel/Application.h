#pragma once

#include "Core.h"

#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Event/Event.h"
#include "Hazel/Event/ApplicationEvent.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();

		virtual ~Application();

		/* 运行Application */
		void Run();

		/* 当Application接收到事件时调用 */
		void OnEvent(Event& e);

		/* 对PushLayer和PushOverlay的封装 */
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		/* 获取当前窗口 */
		inline Window& GetWindow() { return *m_Window; }

		/* 获取Application类的唯一实例 */
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e); // 关闭窗口事件的处理函数

	private:
		std::unique_ptr<Window> m_Window; // 创建一个Window的唯一指针
		bool m_Running = true; // 用于判断Application是否正在运行
		LayerStack m_LayerStack; // 创建一个LayerStack对象，用于管理Layer对象（如：UILayer、GraphicsLayer、SoundLayer等）

	private:
		static Application* s_Instance; // 说明Application类是一个单例模式
	};

	// To be defined in CLIENT
	Application* CreatApplication();

}
