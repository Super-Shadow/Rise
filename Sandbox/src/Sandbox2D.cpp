#include "Sandbox2D.h"

#include <map>

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include <random>

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

	Rise::Renderer2D::ResetStats();

	{
		RS_PROFILE_SCOPE("RenderCommand::Clear");

		Rise::RenderCommand::SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
		Rise::RenderCommand::Clear();
	}

	{
		RS_PROFILE_SCOPE("Renderer2D::Scene");

		Rise::Renderer2D::BeginScene(m_CameraController.GetCamera());

		static auto rotation = 0.f;
		rotation += static_cast<float>(timeStep) * 50.f;
		Rise::Renderer2D::DrawQuad({ 0.f, 1.f }, glm::radians(-45.f), { .8f, .8f }, { 0.8f, 0.2f, 0.3f, 1.f });
		Rise::Renderer2D::DrawQuad({ -1.f, 0.f }, { .8f, .8f }, { 0.8f, 0.2f, 0.3f, 1.f });
		Rise::Renderer2D::DrawQuad({ .5f, -0.5f }, { .5f, .75f }, m_SquareColour);
		Rise::Renderer2D::DrawQuad({0.f, 0.f, -.1f}, { 20.5f, 20.5f }, m_CheckerBoardTexture, 10, glm::vec4(1.f, .9f, .9f, 1.f));
		Rise::Renderer2D::DrawQuad({ 0.f, 0.f, 0.1f }, glm::radians(rotation), { 1.f, 1.f }, m_CheckerBoardTexture);

		for (int i = -5; i < 5; i++)
		{
			for (int j = -5; j < 5; j++)
			{
				auto x = static_cast<float>(i) / 2.f;
				auto y = static_cast<float>(j) / 2.f;
				glm::vec4 colour = { (x + 5.f) / 10.f, .4f, (y + 5.f) / 10.f, .7f };
				Rise::Renderer2D::DrawQuad({ x, y }, { .45f, .45f }, colour);
			}
		}
		Rise::Renderer2D::EndScene();

	}
}

void Sandbox2D::OnImGuiRender()
{
	//using namespace ImGui; TODO: maybe do this?
	RS_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	const auto& stats = Rise::Renderer2D::GetStats(); // i changed this to ref so if it doesnt work set it back to copy

	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Colour", value_ptr(m_SquareColour));

	ImGui::End();
}

void Sandbox2D::OnEvent(Rise::Event& e)
{
	m_CameraController.OnEvent(e);

}
