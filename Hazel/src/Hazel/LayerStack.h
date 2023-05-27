#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	/*
	* 层（Layer）的目的是在给定的时间点上贯穿应用的整个生命周期。
	* 
	* 一个层（Layer）是一个抽象的概念，可以表示任何需要按照一定顺序更新和渲染的东西，比如图形，用户界面，声音等。
	* 一个层栈（LayerStack）是一个容器，它保存了多个层，并允许添加，移除和访问它们。
	* 层栈的顶部是最后一个添加的层，底部是第一个添加的层。
	* 
	*/
	class LayerStack // LayerStack存储和管理不同层次的对象
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer); // 添加一个Layer（普通层）到LayerStack的顶部（前半部分）
		void PushOverlay(Layer* overlay); // 添加一个Layer（覆盖层）到LayerStack的底部（后半部分）
		void PopLayer(Layer* layer); // 从LayerStack顶部移除一个Layer
		void PopOverlay(Layer* overlay); // 从LayerStack底部移除一个Layer

		/* 为了能够在for循环中使用LayerStack，我们需要提供begin和end方法 */
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		// 通过使用指针，类可以存储任何继承自基础Layer类的层类型，并在不知道它们具体类型的情况下调用它们的函数。
		std::vector<Layer*> m_Layers; // 使用vector，因为每一帧都会被迭代
		// 通过使用插入索引，类可以在不使用两个不同容器的情况下，把普通层（前半部分）和覆盖层（后半部分）分开。
		unsigned int m_LayerInsertIndex = 0; // 指向LayerStack中下一个要添加的Layer的索引
	};

}