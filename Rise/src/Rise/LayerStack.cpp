#include "rspch.h"
#include "LayerStack.h"

namespace Rise
{

	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::ranges::find(m_Layers, layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			--m_LayerInsert;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		const auto it = std::ranges::find(m_Layers, overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

}