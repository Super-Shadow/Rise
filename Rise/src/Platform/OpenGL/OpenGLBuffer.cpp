#include "rspch.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Rise
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::size_t size)
	{
		RS_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		// Pass our triangle into buffer. Static draw since it wont change at runtime.
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const std::size_t size)
	{
		RS_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		// Pass our triangle into buffer. Static draw since it wont change at runtime.
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		RS_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		RS_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		RS_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* quadVertex, const size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, quadVertex);
	}

	// TODO: Snus seperate these into their own files 

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const std::size_t count) : m_Count(count)
	{
		RS_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		// https://github.com/TheCherno/Hazel/pull/107 Binding with GL_ARRAY_BUFFER instead of GL_ELEMENT_ARRAY_BUFFER, allows the data to be loaded regardless of VAO state
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); 
		// Pass our drawing order into buffer. Static draw since it wont change at runtime.
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		RS_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		RS_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		RS_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
