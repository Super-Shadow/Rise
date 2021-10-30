#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Rise
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

		RendererAPI(const RendererAPI&) = delete;
		RendererAPI& operator= (const RendererAPI&) = delete;

		RendererAPI(RendererAPI&&) = delete;
		RendererAPI& operator=(RendererAPI&&) = delete;

		virtual ~RendererAPI() = default;

		virtual void Init() abstract;

		virtual void SetViewport(int x, int y, int width, int height) abstract;
		virtual void SetClearColour(const glm::vec4& colour) abstract;
		virtual void Clear() abstract;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, std::size_t indexCount = 0) abstract;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	protected:
		RendererAPI() = default;

	private:
		static API s_API;
	};
}
