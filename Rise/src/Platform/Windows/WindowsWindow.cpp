#include "rspch.h"
#include "WindowsWindow.h"

#include "Rise/Events/ApplicationEvent.h"
#include "Rise/Events/MouseEvent.h"
#include "Rise/Events/KeyEvent.h"
namespace Rise
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int errorCode, const char* description)
	{
		RS_CORE_ERROR("GLFW Error ({0}): {1}", errorCode, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		RS_PROFILE_FUNCTION();

		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		WindowsWindow::Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		RS_PROFILE_FUNCTION();

		WindowsWindow::Shutdown();
	}

	void WindowsWindow:: Init(const WindowProps& props)
	{
		RS_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		// TODO: this is such a non issue, this will only be an error if the requested width or height is over 9,223,372,036,854,775,807.
		// As when we cast a number higher than that to signed int, it will overflow to a negative number.
		// I highly doubt that I will be alive when we have a resolution bigger than that number. :)
		m_Data.Width = static_cast<int>(props.Width) < 0 ? m_Data.Width : static_cast<int>(props.Width);
		m_Data.Height = static_cast<int>(props.Height) < 0 ? m_Data.Height : static_cast<int>(props.Height);

		RS_CORE_INFO("Creating window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (s_GLFWWindowCount == 0)
		{
			RS_PROFILE_SCOPE("glfwInit");

			RS_CORE_INFO("Initializing GLFW");
			[[maybe_unused]] const auto success = glfwInit();
			RS_CORE_ASSERT(success, "Could not initialise GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			RS_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const int width, const int height)
		{
			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			const auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, const int key, int scanCode, const int action, int modes)
		{
			const auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				default:
				{
					RS_CORE_ASSERT(action, "Unhandled glfwSetKeyCallback action!");
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, const unsigned int keyCode)
		{
			const auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(static_cast<int>(keyCode));
			data.EventCallback(event);

		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, const int button, const int action, int mods)
		{
			const auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				default:
				{
					RS_CORE_ASSERT(action, "Unhandled glfwSetMouseButtonCallback action!");
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const double xOffset, const double yOffset)
		{
			const auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Best practice as without cast it is implicit conversion which 'might' cause bugs. This shows explicitly we are converting our double (which is higher precision) to float
			// ReSharper disable twice CppRedundantCastExpression
			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset)); 
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, const double xPos, const double yPos)
		{
			const auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// ReSharper disable twice CppRedundantCastExpression
			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		RS_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);

		if (--s_GLFWWindowCount == 0)
		{
			RS_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		RS_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(const bool enabled)
	{
		RS_PROFILE_FUNCTION();

		glfwSwapInterval(enabled ? 1 : 0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
