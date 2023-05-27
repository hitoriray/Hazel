#pragma once

#include "Hazel/Core.h"

namespace Hazel {

	// �����ֻ࣬��������û��ʵ��
	class HAZEL_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); } // �жϰ����Ƿ���
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); } // �ж���갴���Ƿ���
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); } // �������λ��
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); } // �������x����
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); } // �������y����

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0; // ���麯������Ҫ��ÿ��ƽ̨��ʵ��
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0; // �������λ��
		virtual float GetMouseXImpl() = 0; // �������x����
		virtual float GetMouseYImpl() = 0; // �������y����

	private:
		static Input* s_Instance; // static������������࣬����һ��ȫ�ֱ���
	};
}
