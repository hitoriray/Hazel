#pragma once

#include "Core.h"

#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		/* ����Application */
		void Run();

		/* ��Application���յ��¼�ʱ���� */
		void OnEvent(Event& e);

		/* ��PushLayer��PushOverlay�ķ�װ */
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		/* ��ȡ��ǰ���� */
		inline Window& GetWindow() { return *m_Window; }

		/* ��ȡApplication���Ψһʵ�� */
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e); // �رմ����¼��Ĵ�����
	
	private:
		std::unique_ptr<Window> m_Window; // ����һ��Window��Ψһָ��
		ImGuiLayer* m_ImGuiLayer; // ����һ��ImGuiLayer��ָ��
		bool m_Running = true; // �����ж�Application�Ƿ���������
		LayerStack m_LayerStack; // ����һ��LayerStack�������ڹ���Layer�����磺UILayer��GraphicsLayer��SoundLayer�ȣ�

	private:
		static Application* s_Instance; // ˵��Application����һ������ģʽ
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}