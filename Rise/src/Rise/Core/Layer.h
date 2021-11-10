#pragma once

#include "Rise/Core/TimeStep.h"
#include "Rise/Core/Base.h"
#include "Rise/Events/Event.h"

namespace Rise
{
	class RISE_API Layer
	{
	public:
		explicit Layer(std::string name = "Layer");


		Layer(const Layer&) = delete;
		Layer& operator= (const Layer&) = delete;

		Layer(Layer&&) = delete;
		Layer& operator=(Layer&&) = delete;

		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const TimeStep timeStep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		[[nodiscard]] const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}


