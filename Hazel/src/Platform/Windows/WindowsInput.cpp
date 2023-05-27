#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {

	Input* Input::s_Instance = new WindowsInput(); // 创建一个WindowsInput的实例
	// 由于WindowsInput继承了Input，所以可以将WindowsInput的实例赋值给Input的静态成员变量s_Instance

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()); // 获取窗口指针
		auto state = glfwGetKey(window, keycode); // 获取按键状态
		return state == GLFW_PRESS || state == GLFW_REPEAT; // 判断按键状态是否按下或者重复
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button); // 获取鼠标按键状态
		return state == GLFW_PRESS; // 判断鼠标按键状态是否按下
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos); // 获取鼠标位置
		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl(); // 使用了C++17的新增的结构化绑定
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}