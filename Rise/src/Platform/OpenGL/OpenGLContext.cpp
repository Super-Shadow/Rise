#include "rspch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Rise
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		RS_CORE_ASSERT(windowHandle, "Window handle is null!")

	}

	void OpenGLContext::Init()
	{

		glfwMakeContextCurrent(m_WindowHandle);

		[[maybe_unused]] const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		RS_CORE_ASSERT(status, "Failed to initialise Glad!")

		RS_CORE_INFO("OpenGL Info:");
		RS_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
		RS_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
		RS_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
