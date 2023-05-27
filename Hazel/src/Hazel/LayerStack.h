#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	/*
	* �㣨Layer����Ŀ�����ڸ�����ʱ����ϹᴩӦ�õ������������ڡ�
	* 
	* һ���㣨Layer����һ������ĸ�����Ա�ʾ�κ���Ҫ����һ��˳����º���Ⱦ�Ķ���������ͼ�Σ��û����棬�����ȡ�
	* һ����ջ��LayerStack����һ���������������˶���㣬��������ӣ��Ƴ��ͷ������ǡ�
	* ��ջ�Ķ��������һ����ӵĲ㣬�ײ��ǵ�һ����ӵĲ㡣
	* 
	*/
	class LayerStack // LayerStack�洢�͹���ͬ��εĶ���
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer); // ���һ��Layer����ͨ�㣩��LayerStack�Ķ�����ǰ�벿�֣�
		void PushOverlay(Layer* overlay); // ���һ��Layer�����ǲ㣩��LayerStack�ĵײ�����벿�֣�
		void PopLayer(Layer* layer); // ��LayerStack�����Ƴ�һ��Layer
		void PopOverlay(Layer* overlay); // ��LayerStack�ײ��Ƴ�һ��Layer

		/* Ϊ���ܹ���forѭ����ʹ��LayerStack��������Ҫ�ṩbegin��end���� */
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		// ͨ��ʹ��ָ�룬����Դ洢�κμ̳��Ի���Layer��Ĳ����ͣ����ڲ�֪�����Ǿ������͵�����µ������ǵĺ�����
		std::vector<Layer*> m_Layers; // ʹ��vector����Ϊÿһ֡���ᱻ����
		// ͨ��ʹ�ò���������������ڲ�ʹ��������ͬ����������£�����ͨ�㣨ǰ�벿�֣��͸��ǲ㣨��벿�֣��ֿ���
		unsigned int m_LayerInsertIndex = 0; // ָ��LayerStack����һ��Ҫ��ӵ�Layer������
	};

}