#pragma once
#include "Rise.h"

class ExampleLayer final : public Rise::Layer
{
public:
	ExampleLayer();

	ExampleLayer(const ExampleLayer&) = delete;
	ExampleLayer& operator=(const ExampleLayer&) = delete;

	ExampleLayer(ExampleLayer&&) = delete;
	ExampleLayer& operator=(ExampleLayer&&) = delete;

	~ExampleLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Rise::TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnEvent(Rise::Event & e) override;
private:
	Rise::ShaderLibrary m_ShaderLibrary;

	Rise::Ref<Rise::VertexArray> m_VertexArray;
	Rise::Ref<Rise::VertexArray> m_SquareVertexArray;

	Rise::Ref<Rise::Shader> m_Shader;
	Rise::Ref<Rise::Shader> m_FlatShader;

	Rise::Ref<Rise::Texture2D> m_Texture, m_ChernoTexture;

	Rise::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColour = { 0.2f, 0.3f, 0.8f };
};
