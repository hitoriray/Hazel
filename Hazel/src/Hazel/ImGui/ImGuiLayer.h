#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Event/KeyEvent.h"
#include "Hazel/Event/MouseEvent.h"
#include "Hazel/Event/ApplicationEvent.h"

namespace Hazel {

	// @brief ��Ӧ�ó��������һ��ImGui��ͼ���û�����㣬������ʾһЩ������Ϣ��������壬�˵��ȡ�
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_Time = 0.0f; // �����洢��ǰ��ʱ�䡣���������������ÿһ֡��ʱ�������Ա㴫�ݸ�ImGui����������ṹ�塣
	};

}