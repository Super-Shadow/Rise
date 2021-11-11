#include "Game2DExample.h"

#include <map>

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include <random>
static constexpr int s_MapWidth = 24;
static constexpr const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWDDDDDDWWWWWWWWWWW"
"WWWWWDDDDDDDDDDDWWWWWWWW"
"WWWWDDDDDDDDDDDDDDDWWWWW"
"WWWDDDDDDDDDDDDDDDDDDWWW"
"WWDDDDWWWDDDDDDDDDDDDWWW"
"WDDDDDWWWDDDDDDDDDDDDDWW"
"WWDDDDDDDDDDDDDDDDDDDWWW"
"WWWWDDDDDDDDDDDDDDDDWWWW"
"WWWWWDDDDDDDDDDDDDDWWWWW"
"WWWWWWDDDDDDDDDDDWWWWWWW"
"WWWWWWWDDDDDDDDDWWWWWWWW"
"WWWWWWWWWWDDDDWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWl"
;
static constexpr int s_MapHeight = static_cast<int>(std::char_traits<char>::length(s_MapTiles)) / s_MapWidth;

Game2DExample::Game2DExample() : Layer("Game2DExample"), m_CameraController(1280.f / 720.f)
{
}

void Game2DExample::OnAttach()
{
	RS_PROFILE_FUNCTION();

	m_Particle.ColourBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColourEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 10.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_SpriteSheetTexture = Rise::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_StairsTex = Rise::SubTexture2D::Create(m_SpriteSheetTexture, { 7,6 }, { 128, 128 });
	m_BarrelTex = Rise::SubTexture2D::Create(m_SpriteSheetTexture, { 8,2 }, { 128, 128 });
	m_TreeTex = Rise::SubTexture2D::Create(m_SpriteSheetTexture, { 2,1 }, { 128, 128 }, { 1,2 });

	s_TextureMap['D'] = Rise::SubTexture2D::Create(m_SpriteSheetTexture, { 6,11 }, { 128, 128 });
	s_TextureMap['W'] = Rise::SubTexture2D::Create(m_SpriteSheetTexture, { 11,11 }, { 128, 128 });

	m_CameraController.SetZoomLevel(5.f);
}

void Game2DExample::OnDetach()
{
	RS_PROFILE_FUNCTION();
}

void Game2DExample::OnUpdate(const Rise::TimeStep timeStep)
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


	if (Rise::Input::IsMouseButtonPressed(RS_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Rise::Input::GetMousePosition();
		const auto width = Rise::Application::Get().GetWindow().GetWidth();
		const auto height = Rise::Application::Get().GetWindow().GetHeight();

		const auto bounds = m_CameraController.GetBounds();
		const auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / static_cast<float>(width)) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / static_cast<float>(height)) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(timeStep);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Rise::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (int y = 0; y < s_MapHeight; y++)
	{
		for (int x = 0; x < s_MapWidth; x++)
		{
			auto tileType = s_MapTiles[x + y * s_MapWidth];
			Rise::Ref<Rise::SubTexture2D> texture;

			if (!s_TextureMap[tileType])
				texture = m_BarrelTex; // Missing texture
			else
				texture = s_TextureMap[tileType];

			Rise::Renderer2D::DrawSubTexturedQuad({ .position{ x - s_MapWidth / 2,  s_MapHeight - y - s_MapHeight / 2, .5f }, .subTexture{texture} });
		}
	}

	Rise::Renderer2D::EndScene();
}

void Game2DExample::OnImGuiRender()
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

	ImGui::End();
}

void Game2DExample::OnEvent(Rise::Event& e)
{
	m_CameraController.OnEvent(e);

}
