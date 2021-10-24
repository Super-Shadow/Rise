#include "rspch.h"
#include "Application.h"

#include "Rise/Core/TimeStep.h"
#include "GLFW/glfw3.h"
#include "Rise/Renderer/Renderer.h"

#include <ranges>

namespace Rise
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		RS_PROFILE_FUNCTION();

		RS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(RS_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		RS_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		RS_PROFILE_FUNCTION();

		while (m_Running)
		{
			RS_PROFILE_SCOPE("RunLoop");

			const auto time = static_cast<float>(glfwGetTime()); // TODO: Platform::GetTime
			const TimeStep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(m_UpdateBuffers) // This was in OnWindowResize, but it is called everytime it resized, whereas we only need to update buffer once user has finished resizing. This is dependant on Application::Run() being paused during resizing!
			{
				Renderer::OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());
				m_UpdateBuffers = false;
			}

			if (!m_Minimized)
			{
				RS_PROFILE_SCOPE("LayerStack OnUpdate");
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			{
				RS_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		RS_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(RS_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(RS_BIND_EVENT_FN(Application::OnWindowResize));

		for (const auto& it : std::ranges::reverse_view(m_LayerStack))
		{
			it->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		RS_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		RS_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClosed(const WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		RS_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		m_UpdateBuffers = true;

		return false;
	}
}
