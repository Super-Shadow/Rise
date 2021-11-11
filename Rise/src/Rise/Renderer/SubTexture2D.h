#pragma once
#include "Texture.h"
#include "glm/glm.hpp"

namespace Rise
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		[[nodiscard]] Ref<Texture2D> GetTexture() const { return m_Texture; }
		[[nodiscard]] const glm::vec2* GetTextureCoords() const { return m_TexCoords; }

		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteRatio = {1, 1});

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}
