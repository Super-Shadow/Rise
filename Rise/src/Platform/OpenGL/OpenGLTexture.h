#pragma once
#include "OpenGLShader.h"
#include "Rise/Renderer/Texture.h"

namespace Rise
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(int width, int height);
		explicit OpenGLTexture2D(const std::string& path);

		OpenGLTexture2D(const OpenGLTexture2D&) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;

		OpenGLTexture2D(OpenGLTexture2D&&) = delete;
		OpenGLTexture2D& operator=(OpenGLTexture2D&&) = delete;

		~OpenGLTexture2D() override;

		[[nodiscard]] int GetWidth() const override { return m_Width; }
		[[nodiscard]] int GetHeight() const override { return m_Height; }

		void SetData(void* data, std::size_t size) override;

		void Bind(uint32_t slot = 0) const override;

		bool operator==(const Texture& other) const override { return m_RendererID == dynamic_cast<const OpenGLTexture2D&>(other).m_RendererID; }
	private:
		std::string m_Path;
		int m_Width, m_Height;
		uint32_t m_RendererID{};
		GLenum m_InternalFormat, m_DataFormat;
	};
}
