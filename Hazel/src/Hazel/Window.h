#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"

namespace Hazel {

	struct WindowProps // 用来存储窗口的属性，如标题，宽度，高度等
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hazel Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based window
	class HAZEL_API Window // 窗口的接口，使得具体的窗口系统（如Windows、MaxOS、Linux）实现可以派生自该接口，并提供特定窗口系统的实现
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>; // 表示一个接受Event引用作为参数并返回void的函数类型

		virtual ~Window() {}

		virtual void OnUpdate() = 0; // 表示窗口的更新操作，由派生类实现具体的更新逻辑

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes （窗口属性）
		virtual void SetEventCallBack(const EventCallBackFn& callback) = 0; // 用于设置事件回调函数
		virtual void SetVSync(bool enable) = 0; // 用于设置垂直同步（VSync）功能的开启或关闭
		virtual bool IsVSync() const = 0; // 用于检查垂直同步（VSync）功能是否开启

		// 获取GLFWwindow*
		virtual void* GetNativeWindow() const = 0;
		
		static Window* Create(const WindowProps& props = WindowProps()); // 用于创建窗口对象
		// 使用静态函数Create可以通过传递窗口属性来创建窗口对象，而无需直接实例化特定窗口系统的对象。
		// 这样的设计可以使得代码更加可扩展和可移植，方便在不同的窗口系统上进行切换和使用。
	};

}
