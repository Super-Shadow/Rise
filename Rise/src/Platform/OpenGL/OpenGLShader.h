#pragma once
#include "Rise/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Rise
{
	class OpenGLShader final : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);

		OpenGLShader(const OpenGLShader&) = delete;
		OpenGLShader& operator=(const OpenGLShader&) = delete;

		OpenGLShader(OpenGLShader&&) = delete;
		OpenGLShader& operator=(OpenGLShader&&) = delete;

		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int value) const override;
		void SetFloat(const std::string& name, float value) const override;
		void SetFloat3(const std::string& name, const glm::vec3& value) const override;
		void SetFloat4(const std::string& name, const glm::vec4& value) const override;
		void SetMat4(const std::string& name, const glm::mat4& value) const override;

		[[nodiscard]] const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, int value) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		uint32_t m_RendererID;
		std::string m_Name;
	};
}
