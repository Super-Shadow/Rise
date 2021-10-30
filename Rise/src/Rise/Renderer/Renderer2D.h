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

		// Primitives
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

	};
}
