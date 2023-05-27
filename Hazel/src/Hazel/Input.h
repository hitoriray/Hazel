#pragma once

#include "Hazel/Core.h"

namespace Hazel {

	// 抽象类，只有声明，没有实现
	class HAZEL_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); } // 判断按键是否按下
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); } // 判断鼠标按键是否按下
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); } // 返回鼠标位置
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); } // 返回鼠标x坐标
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); } // 返回鼠标y坐标

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0; // 纯虚函数，需要在每个平台上实现
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0; // 返回鼠标位置
		virtual float GetMouseXImpl() = 0; // 返回鼠标x坐标
		virtual float GetMouseYImpl() = 0; // 返回鼠标y坐标

	private:
		static Input* s_Instance; // static代表不属于这个类，而是一个全局变量
	};
}
