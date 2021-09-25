#pragma once
#include "Rise/Renderer/Texture.h"

namespace Rise
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D() override;

		[[nodiscard]] uint32_t GetWidth() const override { return m_Width; }
		[[nodiscard]] uint32_t GetHeight() const override { return m_Height; }

		void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
}
