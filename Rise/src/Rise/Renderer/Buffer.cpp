#include "rspch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Rise
{
	Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, const std::size_t size)
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:
				RS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		RS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const std::size_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				RS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		RS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
		
	}
}
