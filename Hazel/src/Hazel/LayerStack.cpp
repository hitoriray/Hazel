#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin(); // 初始化m_LayerInsert迭代器，让它指向m_Layers的开头
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers) // 删除m_Layers中的所有层
			delete layer;
	}

	/**
	 * @brief
	 * 一个公有的函数，接受一个指向Layer对象的指针作为参数，并把它添加到m_Layers的前半部分，即m_LayerInsert迭代器之前。
	 * 这个函数用来添加普通层，它们在覆盖层之前被渲染。
	 * 普通层将被推送到列表的前半部分（普通层总是在覆盖层之前，无论是删除还是添加一个普通层）。
	 */
	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer); // 在m_LayerInsert之前插入一个Layer，并更新m_LayerInsert
	}
	
	/**
	 * @brief
	 * 一个公有的函数，接受一个指向Layer对象的指针作为参数，并把它添加到m_Layers的后半部分，即m_LayerInsert迭代器之后。
	 * 这个函数用来添加覆盖层，它们在普通层之上被渲染。(此函数用于添加在普通层之上呈现的叠加层)
	 */
	void LayerStack::PushOverlay(Layer* overlay)
	{
		// 覆盖层将被推送到列表尾部
		m_Layers.emplace_back(overlay); // 在m_Layers的末尾添加一个Layer
	}

	/**
	 * @brief
	 * 一个公有的函数，接受一个指向Layer对象的指针作为参数，并从m_Layers的前半部分移除它，如果它存在的话。
	 * 这个函数也相应地递减（decrements）了m_LayerInsert迭代器。
	 */
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer); // 在m_Layers中查找layer
		if (it != m_Layers.end()) // 如果找到了
		{
			m_Layers.erase(it); // 从m_Layers中移除layer
			m_LayerInsert--; // 更新m_LayerInsert
		}
	}

	/**
	 * @brief 一个公有的函数，接受一个指向Layer对象的指针作为参数，并从m_Layers的后半部分移除它，如果它存在的话。
	 */
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay); // 在m_Layers中查找overlay
		if (it != m_Layers.end()) // 如果找到了
			m_Layers.erase(it); // 从m_Layers中移除overlay
	}

}