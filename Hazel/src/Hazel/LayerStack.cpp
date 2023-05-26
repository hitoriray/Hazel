#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin(); // ��ʼ��m_LayerInsert������������ָ��m_Layers�Ŀ�ͷ
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers) // ɾ��m_Layers�е����в�
			delete layer;
	}

	/**
	 * @brief
	 * һ�����еĺ���������һ��ָ��Layer�����ָ����Ϊ��������������ӵ�m_Layers��ǰ�벿�֣���m_LayerInsert������֮ǰ��
	 * ����������������ͨ�㣬�����ڸ��ǲ�֮ǰ����Ⱦ��
	 * ��ͨ�㽫�����͵��б��ǰ�벿�֣���ͨ�������ڸ��ǲ�֮ǰ��������ɾ���������һ����ͨ�㣩��
	 */
	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer); // ��m_LayerInsert֮ǰ����һ��Layer��������m_LayerInsert
	}
	
	/**
	 * @brief
	 * һ�����еĺ���������һ��ָ��Layer�����ָ����Ϊ��������������ӵ�m_Layers�ĺ�벿�֣���m_LayerInsert������֮��
	 * �������������Ӹ��ǲ㣬��������ͨ��֮�ϱ���Ⱦ��(�˺��������������ͨ��֮�ϳ��ֵĵ��Ӳ�)
	 */
	void LayerStack::PushOverlay(Layer* overlay)
	{
		// ���ǲ㽫�����͵��б�β��
		m_Layers.emplace_back(overlay); // ��m_Layers��ĩβ���һ��Layer
	}

	/**
	 * @brief
	 * һ�����еĺ���������һ��ָ��Layer�����ָ����Ϊ����������m_Layers��ǰ�벿���Ƴ�������������ڵĻ���
	 * �������Ҳ��Ӧ�صݼ���decrements����m_LayerInsert��������
	 */
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer); // ��m_Layers�в���layer
		if (it != m_Layers.end()) // ����ҵ���
		{
			m_Layers.erase(it); // ��m_Layers���Ƴ�layer
			m_LayerInsert--; // ����m_LayerInsert
		}
	}

	/**
	 * @brief һ�����еĺ���������һ��ָ��Layer�����ָ����Ϊ����������m_Layers�ĺ�벿���Ƴ�������������ڵĻ���
	 */
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay); // ��m_Layers�в���overlay
		if (it != m_Layers.end()) // ����ҵ���
			m_Layers.erase(it); // ��m_Layers���Ƴ�overlay
	}

}