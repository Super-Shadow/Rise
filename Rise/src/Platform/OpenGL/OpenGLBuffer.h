#pragma once
#include "Rise/Renderer/Buffer.h"

namespace Rise
{
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const float* vertices, std::size_t size);

		OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
		OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;

		OpenGLVertexBuffer(OpenGLVertexBuffer&&) = delete;
		OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&&) = delete;

		~OpenGLVertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		[[nodiscard]] const BufferLayout& GetLayout() const override { return m_Layout; }
		void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer final : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, std::size_t count);

		OpenGLIndexBuffer(const OpenGLIndexBuffer&) = delete;
		OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = delete;

		OpenGLIndexBuffer(OpenGLIndexBuffer&&) = delete;
		OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&&) = delete;

		~OpenGLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		[[nodiscard]] std::size_t GetCount() const override { return m_Count;  }
	private:
		uint32_t m_RendererID;
		std::size_t m_Count;
	};
}
