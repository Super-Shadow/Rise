#pragma once
#include "Rise/Renderer/RendererAPI.h"

namespace Rise
{
	class OpenGLRendererAPI final : public RendererAPI
	{
	public:
		void Init() override;

		void SetViewport(int x, int y, int width, int height) override;
		void SetClearColour(const glm::vec4& colour) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}
