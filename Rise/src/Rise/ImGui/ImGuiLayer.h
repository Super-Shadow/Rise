#pragma once

#include "Rise/Core/Layer.h"

namespace Rise
{
	class RISE_API ImGuiLayer final : public Layer
	{
	public:
		ImGuiLayer();

		ImGuiLayer(const ImGuiLayer&) = delete;
		ImGuiLayer& operator= (const ImGuiLayer&) = delete;

		ImGuiLayer(ImGuiLayer&&) = delete;
		ImGuiLayer& operator=(ImGuiLayer&&) = delete;

		~ImGuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();
	};
}
