#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Log.h"

#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/MouseEvent.h"
#include "Hazel/Event/KeyEvent.h"

#include <glad/glad.h>

namespace Hazel {

	static bool s_GLFWInitialized = false; // 记录GLFW库是否已经初始化

	static void GLFWErrorCallback(int error, const char* desciption)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, desciption);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props); // 用props中的属性来初始化窗口
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown(); // 销毁窗口
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creatring window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!"); // 仅在Debug版本中出现，因为它可能会终止窗口的创建，但这在Release版本中是不希望出现的
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data); // 设置了窗口的用户指针，用来存储m_Data成员变量的地址，以便在回调函数中访问
		SetVSync(true); // 设置垂直同步

		// Set GLFW callbacks（设置GLFW的回调函数）
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window); // 获取之前存储在UserPoint中的数据，返回void*，要强制转换成(WindowData*)
			data.Width = width; // 更新窗口的宽度和高度
			data.Height = height;

			WindowResizeEvent event(width, height); // 创建窗口调整事件
			data.EventCallBack(event); // 调用回调函数，将事件传递给应用程序
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallBack(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) // 根据按键的动作，创建不同的事件
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallBack(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1); // 第二个参数repeatCount将来需要改变（表示重复次数）
					data.EventCallBack(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallBack(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallBack(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallBack(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xPos, (float)yPos);
			data.EventCallBack(event);
		});
	}

	void Hazel::WindowsWindow::OnUpdate() // Windows系统窗口的更新操作
	{
		glfwSwapBuffers(m_Window); // 交换缓冲区，以实现双缓冲机制，从而显示更新后的图像
		glfwPollEvents(); // 处理窗口事件
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		if (enable) // 启用垂直同步
			glfwSwapInterval(1); // 表示每一帧都等待屏幕刷新
		else // 禁用垂直同步
			glfwSwapInterval(0); // 表示不等待

		m_Data.VSync = enable; // 更新m_Data.VSync的值
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

}