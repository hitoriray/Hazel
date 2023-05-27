#pragma once

#include "Hazel/Input.h"

namespace Hazel {

	class WindowsInput : public Input
	{
	protected:
		// 这里保留virtual是为了增强可读性，因为这个函数是一个虚函数，即使你不写virtual，也改变不了它是以一个虚函数的事实。
		// 但其实这个函数后面有override关键字，很明显就可以知道这个函数重写了一个虚函数。但是把virtual留下可以增强可读性。
		virtual bool IsKeyPressedImpl(int keycode) override; 
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}