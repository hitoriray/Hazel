#pragma once

#include "Hazel/Window.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	class WindowsWindow : public Window // WindowsWindow���ʾһ��Windowsƽ̨�ϵĴ��ڣ��̳���Window
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

		// ��ȡm_Window��void*��ʾ���ص�m_Window��һ����GLFWwindow*���������������͵�ָ�룬����������void*����ʾ
		inline void* GetNativeWindow() const override { return m_Window; }
		
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window; // �����洢һ��GLFWwindow��ָ�룬���ָ��ָ��һ��GLFW���ڣ����������ͨ��GLFW�ⴴ���ģ���һ��ԭ���Ĵ��ڣ�����OpenGL�Ĵ��ڣ�OpenGL�Ĵ�����ͨ��GLFW���ڴ����ġ�

		struct WindowData // ר�ż�¼Windows������
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync; // ��ʾ�Ƿ������˴�ֱͬ��

			EventCallBackFn EventCallBack; // �洢һ���¼��ص������ĵ�ַ����������������������ڷ����ĸ����¼���������¼�������¼��ȡ�
			// �����������ͨ��SetEventCallBack���������ã�Ȼ���ڴ��ڹ����е��á���������ʵ�ִ�������¼�������Ľ���������ԺͿ���չ�ԡ�
		};

		WindowData m_Data; // �����洢������ص����ݣ�����⣬��ȣ��߶ȣ���ֱͬ��״̬���¼��ص�������
	};
}
