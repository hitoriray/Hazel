#pragma once

#include "Hazel/Input.h"

namespace Hazel {

	class WindowsInput : public Input
	{
	protected:
		// ���ﱣ��virtual��Ϊ����ǿ�ɶ��ԣ���Ϊ���������һ���麯������ʹ�㲻дvirtual��Ҳ�ı䲻��������һ���麯������ʵ��
		// ����ʵ�������������override�ؼ��֣������ԾͿ���֪�����������д��һ���麯�������ǰ�virtual���¿�����ǿ�ɶ��ԡ�
		virtual bool IsKeyPressedImpl(int keycode) override; 
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}