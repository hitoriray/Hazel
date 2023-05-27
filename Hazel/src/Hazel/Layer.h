#pragma once

#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"

namespace Hazel {

	// 关于层（Layer）的启用和禁用的宏，以后再实现
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // 当Layer被添加到LayerStack时调用
		virtual void OnDetach() {} // 当Layer从LayerStack中移除时调用
		virtual void OnUpdate() {} // 当Layer被更新时调用（被Application.cpp调用），并且在每一帧中都会被调用
		virtual void OnEvent(Event& event) {} // 当Layer接收到事件时调用
		
		inline const std::string& GetName() const { return m_DebugName; } // 返回Layer的名称

	protected:
		std::string m_DebugName; // Layer的名称 (For Debug)
	};

}
