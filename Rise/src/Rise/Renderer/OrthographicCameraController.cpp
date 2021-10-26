#include "rspch.h"
#include "OrthographicCameraController.h"

#include "Rise/Core/Input.h"
#include "Rise/Core/KeyCodes.h"

namespace Rise
{
	OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool bRotation) : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(bRotation)
	{
		
	}

	void OrthographicCameraController::OnUpdate(const TimeStep timestep)
	{
		RS_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(RS_KEY_W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * static_cast<float>(timestep);
		}
		if (Input::IsKeyPressed(RS_KEY_S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * static_cast<float>(timestep);
		}
		if (Input::IsKeyPressed(RS_KEY_A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * static_cast<float>(timestep);
		}
		if (Input::IsKeyPressed(RS_KEY_D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * static_cast<float>(timestep);
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(RS_KEY_Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * static_cast<float>(timestep);
			}
			if (Input::IsKeyPressed(RS_KEY_E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * static_cast<float>(timestep);
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		if (Input::IsKeyPressed(RS_KEY_R))
		{
			m_CameraPosition = { 0, 0, 0 };
			m_CameraRotation = 0;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel * 2.f;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		RS_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RS_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RS_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& e)
	{
		RS_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& e)
	{
		RS_PROFILE_FUNCTION();

		m_AspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}
