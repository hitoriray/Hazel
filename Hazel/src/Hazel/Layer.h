#pragma once

#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"

namespace Hazel {

	// ���ڲ㣨Layer�������úͽ��õĺ꣬�Ժ���ʵ��
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // ��Layer����ӵ�LayerStackʱ����
		virtual void OnDetach() {} // ��Layer��LayerStack���Ƴ�ʱ����
		virtual void OnUpdate() {} // ��Layer������ʱ���ã���Application.cpp���ã���������ÿһ֡�ж��ᱻ����
		virtual void OnEvent(Event& event) {} // ��Layer���յ��¼�ʱ����
		
		inline const std::string& GetName() const { return m_DebugName; } // ����Layer������

	protected:
		std::string m_DebugName; // Layer������ (For Debug)
	};

}
