#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	// @brief 在应用程序中添加一个ImGui的图形用户界面层，用来显示一些调试信息，控制面板，菜单等。
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override; // 当图层被添加到应用程序时调用
		virtual void OnDetach() override; // 当图层从应用程序中移除时调用
		virtual void OnImGuiRender() override; // 当图层被渲染时调用

		void Begin();
		void End();
	private:
		float m_Time = 0.0f; // 用来存储当前的时间。这个变量用来计算每一帧的时间间隔，以便传递给ImGui的输入输出结构体
	};

}