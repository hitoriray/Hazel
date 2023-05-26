#pragma once

#include "Event.h"

namespace Hazel {

	class HAZEL_API KeyEvent : public Event // KeyEvent是一个抽象类，它被用作KeyPressedEvent和KeyReleasedEvent的基类
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class HAZEL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount; // 记录按键重复的次数
		// 用途：（粗略的解释）
		// 选择一个游戏菜单，只能通过左右键控制，如果没有KeyRepeatEvent则按下一次按键（无论你保持按下状态持续多久），
		// 它只认为你仅仅按下了一次，那么只能移动一个菜单选项；如果有KeyRepeatEvent则会根据你按下按键持续的时间增加
		// m_RepeatCount的次数，以此代表你按下了多少次按键。（当然保持“按下”状态只有超过某个时间，才会增加重复次数）
		// 这个非常有用，例如我们键盘输入一个字母时，按下一次后只输入一个字母，但是如果一直保持“按下”状态，那么会停顿一会后，然后才输入多个字母。
	};

	class HAZEL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class HAZEL_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}
