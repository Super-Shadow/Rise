#pragma once

#include "RendererAPI.h"

namespace Rise
{
	class RenderCommand
	{
	public:
		static void Init() { s_RendererAPI->Init(); }

		static void SetViewport(const int x, const int y, const int width, const int height) { s_RendererAPI->SetViewport(x, y, width, height); }
		static void SetClearColour(const glm::vec4& colour) { s_RendererAPI->SetClearColour(colour); }
		static void Clear() { s_RendererAPI->Clear(); }

		static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
