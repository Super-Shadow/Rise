#include "rspch.h"
#include "SubTexture2D.h"

namespace Rise
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) : m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteRatio)
	{
		const auto sheetWidth = static_cast<float>(texture.get()->GetWidth()), sheetHeight = static_cast<float>(texture.get()->GetHeight());

		glm::vec2 min = { coords.x * cellSize.x / sheetWidth, coords.y * cellSize.y / sheetHeight };
		glm::vec2 max = { (coords.x + spriteRatio.x) * cellSize.x / sheetWidth, (coords.y + spriteRatio.y) * cellSize.y / sheetHeight };

		return CreateRef<SubTexture2D>(texture, min, max);
	}
}
