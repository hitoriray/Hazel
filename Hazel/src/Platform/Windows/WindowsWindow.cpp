#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Log.h"

#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/MouseEvent.h"
#include "Hazel/Event/KeyEvent.h"

#include <glad/glad.h>

namespace Hazel {

	static bool s_GLFWInitialized = false; // ��¼GLFW���Ƿ��Ѿ���ʼ��

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
		Init(props); // ��props�е���������ʼ������
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown(); // ���ٴ���
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
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!"); // ����Debug�汾�г��֣���Ϊ�����ܻ���ֹ���ڵĴ�����������Release�汾���ǲ�ϣ�����ֵ�
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data); // �����˴��ڵ��û�ָ�룬�����洢m_Data��Ա�����ĵ�ַ���Ա��ڻص������з���
		SetVSync(true); // ���ô�ֱͬ��

		// Set GLFW callbacks������GLFW�Ļص�������
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window); // ��ȡ֮ǰ�洢��UserPoint�е����ݣ�����void*��Ҫǿ��ת����(WindowData*)
			data.Width = width; // ���´��ڵĿ�Ⱥ͸߶�
			data.Height = height;

			WindowResizeEvent event(width, height); // �������ڵ����¼�
			data.EventCallBack(event); // ���ûص����������¼����ݸ�Ӧ�ó���
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

			switch (action) // ���ݰ����Ķ�����������ͬ���¼�
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
					KeyPressedEvent event(key, 1); // �ڶ�������repeatCount������Ҫ�ı䣨��ʾ�ظ�������
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

	void Hazel::WindowsWindow::OnUpdate() // Windowsϵͳ���ڵĸ��²���
	{
		glfwSwapBuffers(m_Window); // ��������������ʵ��˫������ƣ��Ӷ���ʾ���º��ͼ��
		glfwPollEvents(); // �������¼�
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		if (enable) // ���ô�ֱͬ��
			glfwSwapInterval(1); // ��ʾÿһ֡���ȴ���Ļˢ��
		else // ���ô�ֱͬ��
			glfwSwapInterval(0); // ��ʾ���ȴ�

		m_Data.VSync = enable; // ����m_Data.VSync��ֵ
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