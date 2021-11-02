#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Rise
{
	class Renderer2D
	{
	public:
		// Delete these to make this class pure singleton
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator=(const Renderer2D&) = delete;

		Renderer2D(Renderer2D&&) = delete;
		Renderer2D& operator=(Renderer2D&&) = delete;
		~Renderer2D() = delete;

		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void FlushAndReset();

		// Primitives
		static void ConstructQuadData(const glm::vec3& position, const float rotation /*Radians*/, const glm::vec2& size, const float textureIndex, const float textureScale, const glm::vec4& colour);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& colour);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float texScale, const glm::vec4& tintColour = {1.f, 1.f, 1.f, 1.f});
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float texScale, const glm::vec4& tintColour = { 1.f, 1.f, 1.f, 1.f });
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float texScale, const glm::vec4& tintColour = { 1.f, 1.f, 1.f, 1.f });
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float texScale, const glm::vec4& tintColour = { 1.f, 1.f, 1.f, 1.f });

		struct Statistics
		{
			uint32_t DrawCalls{ 0 };
			uint32_t QuadCount{ 0 };

			[[nodiscard]] uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			[[nodiscard]] uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

		static Statistics& GetStats();
		static void ResetStats();
	};
}
