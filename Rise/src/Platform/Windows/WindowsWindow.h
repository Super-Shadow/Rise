#pragma once

#include "Rise/Core/Window.h"
#include "Rise/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Rise
{
	class WindowsWindow final :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		WindowsWindow(const WindowsWindow&) = delete;
		WindowsWindow& operator=(const WindowsWindow&) = delete;

		WindowsWindow(WindowsWindow&&) = delete;
		WindowsWindow& operator=(WindowsWindow&&) = delete;

		~WindowsWindow() override;

		void OnUpdate() override;

		[[nodiscard]] int GetWidth() const override { return m_Data.Width; }
		[[nodiscard]] int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		[[nodiscard]] bool IsVSync() const override;

		[[nodiscard]] void* GetNativeWindow() const override { return m_Window; }
	private:
		void Init(const WindowProps& props);
		void Shutdown();

		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			int Width = 1280, Height = 720;
			bool VSync = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}



