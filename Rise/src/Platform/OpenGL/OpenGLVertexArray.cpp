#include "rspch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Rise
{
	static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				return GL_FLOAT;
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return GL_INT;
			case ShaderDataType::Bool:
				return GL_BOOL;
			default:
			{
				RS_CORE_ASSERT(false, "Unknown ShaderDataType!");
				return 0;
			}
		}
	}
	
	OpenGLVertexArray::OpenGLVertexArray()
	{
		RS_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		RS_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		RS_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		RS_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		RS_PROFILE_FUNCTION();

		RS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			// Tell openGL that the bytes just passed in are 3 vec3's
			glEnableVertexAttribArray(m_VertexBufferIndex); // Enable index (first param of next line) of our attrib data

			// First param is the index, next two params are telling it that the bytes are 3 floats. Next param is telling it that they are not normalised.
			// Next param is the amount of bytes between the vertices (So the space for each vertex aka 3(each point for triangle) * 4(size of a float).
			// Last param is the offset for this specific attribute when it is applied to our vertices data thing. We want it to start at the beginning of each vec3 coordinate.
			glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalised ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.Offset));  // NOLINT(performance-no-int-to-ptr)

			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		RS_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
