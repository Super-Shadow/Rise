// ReSharper disable CppClangTidyReadabilityStaticAccessedThroughInstance
#include "rspch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Rise
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TexCoord;
		float TexIndex; // 0 reserved for white texture
		float TilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads{10000};
		const uint32_t MaxVertices{MaxQuads * 4};
		const uint32_t MaxIndices{MaxQuads * 6}; // Since it needs to be a triangle, a square has 6 points to make it into a triangle. Diagonal line through square.
		static constexpr int MaxTextureSlots{32};
		static constexpr glm::vec2 QuadTexCoords[] = { { .0f, .0f }, { 1.f, .0f }, { 1.f, 1.f }, { .0f, 1.f } }; 

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		std::size_t QuadIndexCount{};
		QuadVertex* QuadVertexBufferBase{nullptr};
		QuadVertex* QuadVertexBufferPtr{nullptr};

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots{};
		int TextureSlotIndex{1}; // 0 reserved for white texture

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		RS_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();


		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)); 
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float4, "a_Colour" },
			{ShaderDataType::Float2, "a_TexCoord" },
			{ShaderDataType::Float, "a_TexIndex" },
			{ShaderDataType::Float, "a_TilingFactor" },
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		auto* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		const Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1); // Creates a 1x1 texture
		uint32_t whiteTextureData = 0xFFFFFFFF; // Sets the 1 pixel to be fully white and opaque. Equivalent to 1.f 1.f 1.f 1.f
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int samplers[s_Data.MaxTextureSlots];
		for (auto i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -.5f, -.5f, 0.f, 1.f };
		s_Data.QuadVertexPositions[1] = { .5f, -.5f, 0.f, 1.f };
		s_Data.QuadVertexPositions[2] = { .5f, .5f, 0.f, 1.f };
		s_Data.QuadVertexPositions[3] = { -.5f, .5f, 0.f, 1.f };
	}

	void Renderer2D::Shutdown()
	{
		RS_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RS_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		RS_PROFILE_FUNCTION();

		const std::size_t dataSize = reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferBase);

		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (auto i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void ConstructQuadData(const glm::vec3& position, const float rotation /*Radians*/, const glm::vec2& size, const float textureIndex, const float textureScale, const glm::vec4& colour)
	{
		glm::mat4 transform;
		if (rotation != 0.f) // Rotation costs alot, if not needed don't do it!.
		{
			transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), rotation, { 0.f, 0.f, 1.f }) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		}
		else
		{
			transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });
		}

		for (auto i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Colour = colour;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
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
		ConstructQuadData(position, rotation, size, 0 /*White Texture Index*/, 1.f, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, position.z }, size, texture, 1.f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, position.z }, rotation, size, texture, 1.f);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float texScale, const glm::vec4& tintColour)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture, texScale, tintColour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float texScale, const glm::vec4& tintColour)
	{
		DrawQuad({ position.x, position.y, position.z }, 0.f, size, texture, texScale, tintColour);
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float texScale, const glm::vec4& tintColour)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, texture, texScale, tintColour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation /*Radians*/, const glm::vec2& size, const Ref<Texture2D>& texture, const float texScale, const glm::vec4& tintColour)
	{
		RS_PROFILE_FUNCTION();

		// Try to find the texture if we have already used it, if not add it to our list for future reusing.
		auto textureIndex{ 0.f };

		for (auto i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture.get())
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0)
		{
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		ConstructQuadData(position, rotation, size, textureIndex, texScale, tintColour);
	}


}
