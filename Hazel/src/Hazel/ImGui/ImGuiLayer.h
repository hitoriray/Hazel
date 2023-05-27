#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	// @brief ��Ӧ�ó��������һ��ImGui��ͼ���û�����㣬������ʾһЩ������Ϣ��������壬�˵��ȡ�
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override; // ��ͼ�㱻��ӵ�Ӧ�ó���ʱ����
		virtual void OnDetach() override; // ��ͼ���Ӧ�ó������Ƴ�ʱ����
		virtual void OnImGuiRender() override; // ��ͼ�㱻��Ⱦʱ����

		void Begin();
		void End();
	private:
		float m_Time = 0.0f; // �����洢��ǰ��ʱ�䡣���������������ÿһ֡��ʱ�������Ա㴫�ݸ�ImGui����������ṹ��
	};

}