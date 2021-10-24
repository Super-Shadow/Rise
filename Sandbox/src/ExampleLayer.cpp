#include "ExampleLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

ExampleLayer::ExampleLayer() : Layer("Example"), m_CameraController(1280.f / 720.f, true)
{
	m_VertexArray = Rise::VertexArray::Create();

	// Plot our triange on X Y Z coordinates. X is horizontal and Y is vertical and Z is depth. X is -1 to 1 and Y is -1 bottom and 1 top.
	constexpr float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};
	const Rise::Ref<Rise::VertexBuffer> vertexBuffer = Rise::VertexBuffer::Create(vertices, sizeof vertices);

	const Rise::BufferLayout layout = {
		{Rise::ShaderDataType::Float3, "a_Position" },
		{Rise::ShaderDataType::Float4, "a_Colour" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	constexpr uint32_t indices[3] = { 0, 1, 2 };
	const Rise::Ref<Rise::IndexBuffer> indexBuffer = Rise::IndexBuffer::Create(indices, std::size(indices));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_SquareVertexArray = Rise::VertexArray::Create();

	constexpr float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,	0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	const Rise::Ref<Rise::VertexBuffer> squareVB = Rise::VertexBuffer::Create(squareVertices, sizeof squareVertices);
	squareVB->SetLayout({
		{Rise::ShaderDataType::Float3, "a_Position" },
		{Rise::ShaderDataType::Float2, "a_TexCoord" }
	});
	m_SquareVertexArray->AddVertexBuffer(squareVB);

	constexpr uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	const Rise::Ref<Rise::IndexBuffer> squareIB = Rise::IndexBuffer::Create(squareIndices, std::size(squareIndices));
	m_SquareVertexArray->SetIndexBuffer(squareIB);

	const std::string vertexSrc = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				v_Colour = a_Colour;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
	const std::string pixelSrc = R"(
			#version 460 core

			layout(location = 0) out vec4 colour;

			in vec3 v_Position;
			in vec4 v_Colour;

			void main()
			{
				//colour = vec4(0.8, 0.2, 0.3, 1.0);
				colour = vec4(v_Position * 0.5 + 0.5, 1.0);
				colour = v_Colour;
			}
		)";
	m_Shader = Rise::Shader::Create("VertexPosColour", vertexSrc, pixelSrc);
	// -------------------------------------------------------------------------------------

	const std::string flatVertexSrc = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
	const std::string flatPixelSrc = R"(
			#version 460 core

			layout(location = 0) out vec4 colour;

			in vec3 v_Position;

			uniform vec3 u_Colour;

			void main()
			{
				colour = vec4(u_Colour, 1.0);
			}
		)";
	m_FlatShader = Rise::Shader::Create("FlatColour", flatVertexSrc, flatPixelSrc);
	// -------------------------------------------------------------------------------------

	const auto texture = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	// -------------------------------------------------------------------------------------

	m_Texture = Rise::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoTexture = Rise::Texture2D::Create("assets/textures/ChernoLogo.png");

	texture->Bind();
	texture->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{

}

void ExampleLayer::OnDetach()
{
	
}

void ExampleLayer::OnUpdate(const Rise::TimeStep timeStep)
{
	RS_TRACE("Delta time: {0}s ({1}ms)", timeStep.GetSeconds(), timeStep.GetMilliseconds());

	m_CameraController.OnUpdate(timeStep);

	Rise::RenderCommand::SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
	Rise::RenderCommand::Clear();

	Rise::Renderer::BeginScene(m_CameraController.GetCamera());

	const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatShader->Bind();
	m_FlatShader->SetFloat3("u_Colour", m_SquareColour);

	for (float y = 0; y < 20; ++y)
	{
		for (float x = 0; x < 20; ++x)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			const glm::mat4 transform = translate(glm::mat4(1.0f), pos) * scale;
			// Draws our square
			Rise::Renderer::Submit(m_FlatShader, m_SquareVertexArray, transform);
		}
	}

	const auto textureShader = m_ShaderLibrary.Get("Texture");

	m_Texture->Bind();
	Rise::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	m_ChernoTexture->Bind();
	Rise::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Draws our triangle
	//Rise::Renderer::Submit(m_Shader, m_VertexArray);

	Rise::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Colour", value_ptr(m_SquareColour));
	ImGui::End();
}

void ExampleLayer::OnEvent(Rise::Event& e)
{
	m_CameraController.OnEvent(e);
}