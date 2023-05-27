#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {

	Input* Input::s_Instance = new WindowsInput(); // ����һ��WindowsInput��ʵ��
	// ����WindowsInput�̳���Input�����Կ��Խ�WindowsInput��ʵ����ֵ��Input�ľ�̬��Ա����s_Instance

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()); // ��ȡ����ָ��
		auto state = glfwGetKey(window, keycode); // ��ȡ����״̬
		return state == GLFW_PRESS || state == GLFW_REPEAT; // �жϰ���״̬�Ƿ��»����ظ�
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button); // ��ȡ��갴��״̬
		return state == GLFW_PRESS; // �ж���갴��״̬�Ƿ���
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos); // ��ȡ���λ��
		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl(); // ʹ����C++17�������Ľṹ����
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}