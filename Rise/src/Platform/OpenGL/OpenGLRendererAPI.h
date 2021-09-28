#pragma once
#include "Rise/Renderer/RendererAPI.h"

namespace Rise
{
	class OpenGLRendererAPI final : public RendererAPI
	{
	public:
		void Init() override;

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void SetClearColour(const glm::vec4& colour) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}
