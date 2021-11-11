#pragma once
#include "OrthographicCamera.h"
#include "SubTexture2D.h"
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

		static void ConstructQuadData(const glm::vec3& position, float rotation /*Radians*/, const glm::vec2& size, float textureIndex, const glm::vec2 textureCoords[], float textureScale, const glm::vec4& colour);

		// Primitives
		struct DrawQuadParams
		{
			glm::vec3 position{0.f};
			float rotation{0.f};
			glm::vec2 size{1.f};
			float textureScale{1.f};
			glm::vec4 tintColour{ 1.f, 1.f, 1.f, 1.f };
		};

		struct DrawTexturedQuadParams
		{
			glm::vec3 position{ 0.f };
			float rotation{ 0.f };
			glm::vec2 size{ 1.f };
			Ref<Texture2D> texture = nullptr;
			float textureScale{ 1.f };
			glm::vec4 tintColour{ 1.f, 1.f, 1.f, 1.f };
		};

		static void DrawQuad(const DrawQuadParams&& params); // Use && to move by R(ight)-value to encourage users to use aggregate initialisation and maybe more performant that just by copy?
		static void DrawTexturedQuad(const DrawTexturedQuadParams&& params);

		struct DrawSubTexQuadParams
		{
			glm::vec3 position{ 0.f };
			float rotation{ 0.f };
			glm::vec2 size{ 1.f };
			Ref<SubTexture2D> subTexture = nullptr;
			float textureScale{ 1.f };
			glm::vec4 tintColour{ 1.f, 1.f, 1.f, 1.f };
		};
		static void DrawSubTexturedQuad(const DrawSubTexQuadParams&& params);

		static float BatchTexture(const Ref<Texture2D>& texture);

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
