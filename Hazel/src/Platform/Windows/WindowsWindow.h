#pragma once

#include "Hazel/Window.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	class WindowsWindow : public Window // WindowsWindow类表示一个Windows平台上的窗口，继承自Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallBack(const EventCallBackFn& callback) override { m_Data.EventCallBack = callback; }
		void SetVSync(bool enable) override;
		bool IsVSync() const override;

		// 获取m_Window，void*表示返回的m_Window不一定是GLFWwindow*，可能是其他类型的指针，所以这里用void*来表示
		inline void* GetNativeWindow() const override { return m_Window; }
		
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window; // 用来存储一个GLFWwindow的指针，这个指针指向一个GLFW窗口，这个窗口是通过GLFW库创建的，是一个原生的窗口，不是OpenGL的窗口，OpenGL的窗口是通过GLFW窗口创建的。

		struct WindowData // 专门记录Windows的数据
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync; // 表示是否启用了垂直同步

			EventCallBackFn EventCallBack; // 存储一个事件回调函数的地址，这个函数可以用来处理窗口发生的各种事件，如键盘事件，鼠标事件等。
			// 这个变量可以通过SetEventCallBack函数来设置，然后在窗口过程中调用。这样可以实现窗口类和事件处理类的解耦，提高灵活性和可扩展性。
		};

		WindowData m_Data; // 用来存储窗口相关的数据，如标题，宽度，高度，垂直同步状态，事件回调函数等
	};
}
