#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) // ����һ���꣬���ڰ��¼�

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create()); // ����һ��Window��Ψһָ��
		m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent)); // ���¼��ص�����

		//unsigned int id;
		//glGenVertexArrays(1, &id); // �õ���һ����Ч��id��֤������Glad�ɹ�

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	// ------- Ӧ�ó������ ---------------------------------------
	void Application::Run()
	{
		while (m_Running)
		{
			//glClearColor(1, 0, 1, 1); // ���ô�����ɫΪ��ɫ
			glClear(GL_COLOR_BUFFER_BIT); // �����ɫ������

			for (Layer* layer : m_LayerStack) // ����LayerStack������ÿһ��Layer
				layer->OnUpdate();

			//auto [x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0}, {1}", x, y);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate(); // ���´���
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

	void Application::OnEvent(Event& e) // �����¼��Ĵ�����
	{
		EventDispatcher dispatcher(e); // ����һ���¼��ַ���
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); // �󶨹رմ����¼�

		//HZ_CORE_TRACE("{0}", e); // ���������¼�

		// �������LayerStack�������ո��ǲ����ȵ�ԭ���������¼������README.md��
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled()) // ����¼��������ˣ�������ѭ�����������´��ݣ����������Ĳ�Ͳ���Ҫ�ٴ�������¼��ˣ�
				break;
		}
	}

	
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}