#pragma once
#include "Rise.h"

class Sandbox2D final : public Rise::Layer
{
public:
	Sandbox2D();

	Sandbox2D(const Sandbox2D&) = delete;
	Sandbox2D& operator=(const Sandbox2D&) = delete;

	Sandbox2D(Sandbox2D&&) = delete;
	Sandbox2D& operator=(Sandbox2D&&) = delete;

	~Sandbox2D() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Rise::TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnEvent(Rise::Event& e) override;
private:
	Rise::OrthographicCameraController m_CameraController;

	// TODO: Temporary
	Rise::Ref<Rise::VertexArray> m_SquareVertexArray;
	Rise::Ref<Rise::Shader> m_FlatColourShader;

	Rise::Ref<Rise::Texture2D> m_CheckerBoardTexture;

	glm::vec4 m_SquareColour = { 0.2f, 0.3f, 0.8f, 1.0f };
};
