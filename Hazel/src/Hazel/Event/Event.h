#pragma once

#include "Hazel/Core.h"

#include "hzpch.h"

namespace Hazel {

	/*
		Events in Hazel are currently blocking, meaning when an event occurs it
		immediately gets dispatched and must be dealt with right then and there.
		For the future, a better strategy might be to buffer events in an event
		bus and process them during the "event" part of the update stage.
	*/

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		// BIT(x) = 1 << x �� 1 ���� x λ
		// �����÷���C++�н���λ����(bit mask)
		EventCategoryApplication	= BIT(0), // BIT(0) = 0000 0001 ��1
		EventCategoryInput			= BIT(1), // BIT(1) = 0000 0010 ��2
		EventCategoryKeyboard		= BIT(2), // BIT(2) = 0000 0100 ��4
		EventCategoryMouse			= BIT(3), // BIT(3) = 0000 1000 ��8
		EventCategoryMouseButton	= BIT(4)  // BIT(4) = 0001 0000 ��16
	};
	// ��������������������ʵ��GetEventType()��GetName()��GetCategoryFlags()���������麯����
	// ������Ƶ�ԭ����Ϊ�˱����ظ�д��ͬ�Ĵ��룬��߿ɶ��ԺͿ�ά���ԡ�
	// ���У�GetStaticType()��������ȡ��ǰ�¼������͡���GetStaticType()���ó�static��ԭ��
	// Ϊ�����������ڲ���Ҫ�������������µ��ã��������¼��ַ����У����ǿ�����Ҫ�����¼���������ѡ��ͬ�Ĵ�������������Ҫ֪��������¼�����
	// ���磺����EventDispatcher����Dispatch()����ԭ������֡�
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	/*
		EVENT_CLASS_TYPE(KeyPressed)չ��������ӣ�
			static EventType GetStaticType() { return EventType::KeyPressed; }
			virtual EventType GetEventType() const override { return GetStaticType(); }
			virtual const char* GetName() const override { return "KeyPressed"; }
		EVENT_CLASS_CATEGORY(EventCategoryApplication)չ����
			virtual int GetCategoryFlags() const override { return EventCategoryApplication; }
	*/

	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0; // ��ȡ���¼������ͣ��磺�����¼�������¼��ȣ�
		virtual const char* GetName() const = 0; // ��ȡ���¼������ƣ��磺KeyPressedEvent��MouseMovedEvent�ȣ�
		virtual int GetCategoryFlags() const = 0; // ��ȡ���¼�����������磺�������͡��������ȣ����ص���һ��������ÿһλ��Ӧ��һ����𣬱���EventCategoryInput = BIT(1)��
		virtual std::string ToString() const { return GetName(); } // �����¼�ת�����ַ�����Ĭ�Ϸ����¼�������

		inline bool IsInCategory(EventCategory category) // �жϸ��¼��Ƿ�����category�����������õ���λ���������GetCategoryFlags()���ص������е�ĳһλ�Ƿ�Ϊ1��
		{ 
			return GetCategoryFlags() & category; 
		} 

		inline bool IsHandled() const { return m_Handled; } // 

	protected:
		bool m_Handled = false; // �жϸ��¼��Ƿ��Ѿ����������
	};

	class EventDispatcher // ��������ã�����ͬ���͵��¼����¼����������Լ��ַ��¼�����Ӧ�ļ�������
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>; // ��ʾһ������ĳ�����ͣ�T�����¼���Ϊ����������boolֵ�ĺ�������
	public:
		EventDispatcher(Event& event) // Ĭ�Ϲ��캯�����Խ��������Event
			: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template<typename T>
		bool Dispatch(EventFn<T> func) // һ��ģ�庯���������ַ��¼���ָ���ĺ�������
		{	// EventFn<T> func ����ʾҪ���õĺ�������
			if (m_Event.GetEventType() == T::GetStaticType()) // �����ж�m_Event�������Ƿ��T��ͬ
			{
				// �����ͬ���Ͱ�m_Eventת����T���Ͳ�����func������func�ķ���ֵ����m_Event��m_Handled����
				//m_Event.m_Handled = func(static_cast<T&>(m_Event));
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event; // ���ڽ����������͵�Event
	}; // ������Ƶ�ԭ����Ϊ���ò�ͬ���͵��¼�����������ע�ᵽEventDispatcher�У����Ҹ����¼���������ѡ����ʵļ������������¼���

	// �ú���ʱΪ��־������ڵģ�ʹ�����ǿ��Ժ����׵����¼��ϵ���ToString()
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
