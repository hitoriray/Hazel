#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) // 定义一个宏，用于绑定事件

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create()); // 创建一个Window的唯一指针
		m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent)); // 绑定事件回调函数

		//unsigned int id;
		//glGenVertexArrays(1, &id); // 得到了一个有效的id，证明配置Glad成功

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	// ------- 应用程序入口 ---------------------------------------
	void Application::Run()
	{
		while (m_Running)
		{
			//glClearColor(1, 0, 1, 1); // 设置窗口颜色为粉色
			glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区

			for (Layer* layer : m_LayerStack) // 遍历LayerStack，更新每一个Layer
				layer->OnUpdate();

			//auto [x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0}, {1}", x, y);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate(); // 更新窗口
		}
	}
	// -----------------------------------------------------------

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) // 所有事件的处理函数
	{
		EventDispatcher dispatcher(e); // 创建一个事件分发器
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); // 绑定关闭窗口事件

		//HZ_CORE_TRACE("{0}", e); // 跟踪所有事件

		// 逆序遍历LayerStack，并按照覆盖层优先的原则来处理事件（详见README.md）
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled()) // 如果事件被处理了，就跳出循环，不再向下传递（这样其他的层就不需要再处理这个事件了）
				break;
		}
	}

	
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}