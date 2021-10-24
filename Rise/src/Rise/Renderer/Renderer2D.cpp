#include "rspch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Rise
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		RS_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		constexpr float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.f, 0.f,
			 0.5f, -0.5f, 0.0f, 1.f, 0.f,
			 0.5f,	0.5f, 0.0f, 1.f, 1.f,
			-0.5f,  0.5f, 0.0f, 0.f, 1.f
		};

		const Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof squareVertices);
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float2, "a_TexCoord" },
		});

		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		constexpr uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		const Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, std::size(squareIndices));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1); // Creates a 1x1 texture
		uint32_t whiteTextureData = 0xFFFFFFFF; // Sets the 1 pixel to be fully white and opaque. Equivalent to 1.f 1.f 1.f 1.f
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
		//s_Data->TextureShader->SetInt("u_Bayer8", 1);
	}

	void Renderer2D::Shutdown()
	{
		RS_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RS_PROFILE_FUNCTION();

		//		dynamic_cast<OpenGLShader*>(s_Data->FlatColourShader.get())->Bind(); if FlatColourShader is Scope<Shader>

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		RS_PROFILE_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour)
	{
		DrawQuad(position, 0.f, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation /*Radians*/, const glm::vec2& size, const glm::vec4& colour)
	{
		RS_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Colour", colour);
		s_Data->WhiteTexture->Bind();


		glm::mat4 transform;

		if (rotation != 0.f) // Rotation costs alot, if not needed don't do it!.
		{
			transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), rotation, { 0.f, 0.f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		}
		else
		{
			transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		}

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, position.z }, size, texture, 1.f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, position.z }, rotation, size, texture, 1.f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, const float texScale, const glm::vec4& tintColour)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture, texScale, tintColour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const float texScale, const glm::vec4& tintColour)
	{
		DrawQuad({ position.x, position.y, position.z }, 0.f, size, texture, texScale, tintColour);
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<Texture>& texture, const float texScale, const glm::vec4& tintColour)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, texture, texScale, tintColour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation /*Radians*/, const glm::vec2& size, const Ref<Texture>& texture, const float texScale, const glm::vec4& tintColour)
	{
		RS_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Colour", tintColour);
		s_Data->TextureShader->SetFloat("u_TexScale", texScale);

		glm::mat4 transform;
		
		if(rotation != 0.f) // Rotation costs alot, if not needed don't do it!.
		{
			transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), rotation,{ 0.f, 0.f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		}
		else
		{
			transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		}
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		/*
		static auto Bayer8 = Texture2D::Create("assets/textures/bayer8.png");
		Bayer8->Bind(1);*/

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}
