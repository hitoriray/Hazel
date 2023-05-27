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
		// BIT(x) = 1 << x 即 1 左移 x 位
		// 这种用法在C++中叫做位掩码(bit mask)
		EventCategoryApplication	= BIT(0), // BIT(0) = 0000 0001 即1
		EventCategoryInput			= BIT(1), // BIT(1) = 0000 0010 即2
		EventCategoryKeyboard		= BIT(2), // BIT(2) = 0000 0100 即4
		EventCategoryMouse			= BIT(3), // BIT(3) = 0000 1000 即8
		EventCategoryMouseButton	= BIT(4)  // BIT(4) = 0001 0000 即16
	};
	// 下面这两个个宏是用来实现GetEventType()，GetName()和GetCategoryFlags()这三个纯虚函数。
	// 这样设计的原因是为了避免重复写相同的代码，提高可读性和可维护性。
	// 其中，GetStaticType()是用来获取当前事件的类型。把GetStaticType()设置成static的原因：
	// 为了让它可以在不需要创建对象的情况下调用，比如在事件分发器中，我们可能需要根据事件的类型来选择不同的处理函数，而不需要知道具体的事件对象。
	// 例如：下面EventDispatcher类中Dispatch()就是原因的体现。
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	/*
		EVENT_CLASS_TYPE(KeyPressed)展开后的样子：
			static EventType GetStaticType() { return EventType::KeyPressed; }
			virtual EventType GetEventType() const override { return GetStaticType(); }
			virtual const char* GetName() const override { return "KeyPressed"; }
		EVENT_CLASS_CATEGORY(EventCategoryApplication)展开后：
			virtual int GetCategoryFlags() const override { return EventCategoryApplication; }
	*/

	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0; // 获取该事件的类型（如：键盘事件、鼠标事件等）
		virtual const char* GetName() const = 0; // 获取该事件的名称（如：KeyPressedEvent、MouseMovedEvent等）
		virtual int GetCategoryFlags() const = 0; // 获取该事件所属的类别（如：输入类型、键盘类别等）返回的是一个整数，每一位对应着一个类别，比如EventCategoryInput = BIT(1)等
		virtual std::string ToString() const { return GetName(); } // 将该事件转换成字符串，默认返回事件的名称

		inline bool IsInCategory(EventCategory category) // 判断该事件是否属于category类别（这个函数用到了位运算来检查GetCategoryFlags()返回的整数中的某一位是否为1）
		{ 
			return GetCategoryFlags() & category; 
		} 

		inline bool IsHandled() const { return m_Handled; } // 

	protected:
		bool m_Handled = false; // 判断该事件是否已经被处理过了
	};

	class EventDispatcher // 该类的作用：管理不同类型的事件和事件监听器，以及分发事件给相应的监听器。
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>; // 表示一个接受某种类型（T）的事件作为参数并返回bool值的函数对象
	public:
		EventDispatcher(Event& event) // 默认构造函数可以接收任意的Event
			: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template<typename T>
		bool Dispatch(EventFn<T> func) // 一个模板函数，用来分发事件给指定的函数对象
		{	// EventFn<T> func ：表示要调用的函数对象
			if (m_Event.GetEventType() == T::GetStaticType()) // 首先判断m_Event的类型是否和T相同
			{
				// 如果相同，就把m_Event转换成T类型并传给func，并把func的返回值赋给m_Event的m_Handled变量
				//m_Event.m_Handled = func(static_cast<T&>(m_Event));
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event; // 用于接收任意类型的Event
	}; // 这样设计的原因是为了让不同类型的事件监听器都能注册到EventDispatcher中，并且根据事件的类型来选择合适的监听器来处理事件。

	// 该函数时为日志库而存在的，使得我们可以很容易的在事件上调用ToString()
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
