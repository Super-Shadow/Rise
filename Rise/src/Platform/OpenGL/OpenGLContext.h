#pragma once
#include "Rise/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Rise
{
	class OpenGLContext final : public GraphicsContext
	{
	public:
		explicit OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
