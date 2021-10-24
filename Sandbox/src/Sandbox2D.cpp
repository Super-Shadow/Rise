#include "Sandbox2D.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{
}

void Sandbox2D::OnAttach()
{
	RS_PROFILE_FUNCTION();

	m_CheckerBoardTexture = Rise::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	RS_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(const Rise::TimeStep timeStep)
{
	RS_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(timeStep);

	// Render
	{
		RS_PROFILE_SCOPE("RenderCommand::Clear");

		Rise::RenderCommand::SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
		Rise::RenderCommand::Clear();
	}

	{
		RS_PROFILE_SCOPE("Renderer2D::Scene");

		Rise::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Rise::Renderer2D::DrawQuad({ -1.f, 0.f }, { .8f, .8f }, { 0.8f, 0.2f, 0.3f, 1.f });
		Rise::Renderer2D::DrawQuad({ .5f, -0.5f }, { .5f, .75f }, { 0.2f, 0.3f, 0.8f, 1.f });
		Rise::Renderer2D::DrawQuad({ 0.f, 0.f, -0.1f }, { 10.5f, 10.5f }, m_CheckerBoardTexture, 10);

		Rise::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	RS_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Colour", value_ptr(m_SquareColour));
	ImGui::End();
}

void Sandbox2D::OnEvent(Rise::Event& e)
{
	m_CameraController.OnEvent(e);

}
