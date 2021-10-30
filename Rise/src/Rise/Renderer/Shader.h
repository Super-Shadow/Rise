#pragma once
#include <glm/glm.hpp>

namespace Rise
{
	class Shader
	{
	public:

		Shader(const Shader&) = delete;
		Shader& operator= (const Shader&) = delete;

		Shader(Shader&&) = delete;
		Shader& operator=(Shader&&) = delete;

		virtual ~Shader() = default;

		virtual void Bind() const abstract;
		virtual void Unbind() const abstract;

		virtual void SetInt(const std::string& name, int value) const abstract;
		virtual void SetIntArray(const std::string& name, const int* values, int count) const abstract;
		virtual void SetFloat(const std::string& name, float value) const abstract;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) const abstract;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) const abstract;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) const abstract;

		[[nodiscard]] virtual const std::string& GetName() const abstract;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);

	protected:
		Shader() = default;
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filePath); // Gets name from filename.
		Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);

		[[nodiscard]] bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
