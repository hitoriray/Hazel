#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	struct WindowProps // �����洢���ڵ����ԣ�����⣬��ȣ��߶ȵ�
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

	// Interface representing a desktop system based Window
	class HAZEL_API Window // ���ڵĽӿڣ�ʹ�þ���Ĵ���ϵͳ����Windows��MaxOS��Linux��ʵ�ֿ��������Ըýӿڣ����ṩ�ض�����ϵͳ��ʵ��
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>; // ��ʾһ������Event������Ϊ����������void�ĺ�������

		virtual ~Window() {} // �����ͷŴ��ڶ������Դ

		virtual void OnUpdate() = 0; // ��ʾ���ڵĸ��²�������������ʵ�־���ĸ����߼�

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes ���������ԣ�
		virtual void SetEventCallBack(const EventCallbackFn& callback) = 0; // ���������¼��ص�����
		virtual void SetVSync(bool enabled) = 0; // �������ô�ֱͬ����VSync�����ܵĿ�����ر�
		virtual bool IsVSync() const = 0; // ���ڼ�鴹ֱͬ����VSync�������Ƿ���

		virtual void* GetNativeWindow() const = 0; // ���ڻ�ȡ���ڵ�ԭ��ָ��

		static Window* Create(const WindowProps& props = WindowProps()); // ���ڴ������ڶ���
		// ʹ�þ�̬����Create����ͨ�����ݴ����������������ڶ��󣬶�����ֱ��ʵ�����ض�����ϵͳ�Ķ���
		// ��������ƿ���ʹ�ô�����ӿ���չ�Ϳ���ֲ�������ڲ�ͬ�Ĵ���ϵͳ�Ͻ����л���ʹ�á�
	};

}