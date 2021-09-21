#pragma once

#include "Core/Timestep.h"
#include "Rise/Core.h"
#include "Rise/Events/Event.h"

namespace Rise
{
	class RISE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		[[nodiscard]] const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}


