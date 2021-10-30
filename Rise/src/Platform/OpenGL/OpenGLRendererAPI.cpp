#include "rspch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Rise
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		const unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
			{
				RS_CORE_CRITICAL(message);
				return;
			}
			case GL_DEBUG_SEVERITY_MEDIUM:
			{
				RS_CORE_ERROR(message);
				return;
			}
			case GL_DEBUG_SEVERITY_LOW:
			{
				RS_CORE_WARN(message);
				return;
			}
			case GL_DEBUG_SEVERITY_NOTIFICATION:
			{
				RS_CORE_TRACE(message);
				return;
			}
		
		}

		RS_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
		RS_PROFILE_FUNCTION();

		#ifdef RS_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(const int x, const int y, const int width, const int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColour(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const std::size_t indexCount)
	{
		const std::size_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount(); // TODO: Change this whole thing to an int to remove unsinged to signed cast, though this API will be obsolete in favour of vulcan or directx12
		glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr); // Can pass nullptr for indices as they are tied to the same glGenBuffers(1, ...) so it just is automatically linked.
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
