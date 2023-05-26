#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Event/KeyEvent.h"
#include "Hazel/Event/MouseEvent.h"
#include "Hazel/Event/ApplicationEvent.h"

namespace Hazel {

	// @brief 在应用程序中添加一个ImGui的图形用户界面层，用来显示一些调试信息，控制面板，菜单等。
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
		float m_Time = 0.0f; // 用来存储当前的时间。这个变量用来计算每一帧的时间间隔，以便传递给ImGui的输入输出结构体。
	};

}