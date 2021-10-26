#pragma once
#include "Rise/Core/TimeStep.h"
#include "Rise/Events/ApplicationEvent.h"
#include "Rise/Events/Event.h"
#include "Rise/Events/MouseEvent.h"
#include "Rise/Renderer/OrthographicCamera.h"

namespace Rise
{
	class OrthographicCameraController
	{
	public:
		explicit OrthographicCameraController(float aspectRatio, bool bRotation = false);

		void OnUpdate(TimeStep timestep);
		void OnEvent(Event& e);

		[[nodiscard]] OrthographicCamera& GetCamera() { return m_Camera; }
		[[nodiscard]] const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(const float level) { m_ZoomLevel = level; }
		[[nodiscard]] float GetZoomLevel() const { return m_ZoomLevel; }

	private:
		bool OnMouseScrolled(const MouseScrolledEvent& e);
		bool OnWindowResized(const WindowResizeEvent& e);

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition{0.0f};
		float m_CameraTranslationSpeed = 2.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 100.0f;
	};
}
