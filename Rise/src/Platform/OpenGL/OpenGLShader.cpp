#include "rspch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <utility>

#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

namespace Rise
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		RS_CORE_ASSERT(false, "Unknown shader type '" + type + "'!");
		return NULL;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		RS_PROFILE_FUNCTION();

		const auto source = ReadFile(filePath);
		const auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// File name from file path
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		const auto lastDot = filePath.rfind('.');
		const auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(std::move(name))
	{
		RS_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		RS_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		RS_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			const auto size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				RS_CORE_ERROR("Could not read from file '{0}'", filePath);
			}
		}
		else
		{
			RS_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		RS_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		constexpr auto typeToken = "#type";
		//const auto typeTokenLength = strlen(typeToken);
		auto pos = source.find(typeToken, 0);
		RS_CORE_ASSERT(pos != std::string::npos, "Shader missing #type!");
		while (pos != std::string::npos)
		{
			const auto endOfLine = source.find_first_of("\r\n", pos);
			RS_CORE_ASSERT(endOfLine != std::string::npos, "Syntax error");

			const auto begin = pos + /*typeTokenLength + 1 */ 6; // The plus 1 is how many whitespaces between '#type vertex'
			auto type = source.substr(begin, endOfLine - begin);
			RS_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification!"); // TODO: This is useless due to exact same asset in ShaderTypeFromString

			const auto nextLinePos = source.find_first_not_of("\r\n", endOfLine);
			RS_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		RS_PROFILE_FUNCTION();

		// Now time to link them together into a program.
		// Get a program object.
		const auto program = glCreateProgram();
		RS_CORE_ASSERT(shaderSources.size() <= 2, "Unsupported amount of shaders!");
		std::array<GLenum, 2> glShaderIDs{};
		auto glShaderIDIndex = 0;
		for (const auto& [first, second] : shaderSources) // Good use of structural binding!
		{
			const auto type = first;
			const auto source = second;

			// Create an empty vertex shader handle
			const auto shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			auto sourceCstr = source.c_str();
			glShaderSource(shader, 1, &sourceCstr, nullptr);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// Either of them. Don't leak shaders.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				RS_CORE_ERROR("{0}", infoLog.data());
				RS_CORE_ASSERT(false, "Shader compilation failure!");

				// In this simple program, we'll just leave
				break;
			}
			// Vertex and fragment shaders are successfully compiled.
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (const auto shader : glShaderIDs)
			{
				glDeleteShader(shader);
			}

			// Use the infoLog as you see fit.
			RS_CORE_ERROR("{0}", infoLog.data());
			RS_CORE_ASSERT(false, "Shader link failure!");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (const auto shader : glShaderIDs)
		{
			glDetachShader(program, shader);
		}

		m_RendererID = program;
	}


	void OpenGLShader::Bind() const
	{
		RS_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		RS_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, const int value) const
	{
		RS_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, const int* values, const int count) const
	{
		RS_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value) const
	{
		RS_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) const
	{
		RS_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) const
	{
		RS_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		RS_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if(location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform int named {1}.", m_Name, name);

		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, const int* values, const int count) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform int array named {1}.", m_Name, name);

		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform float named {1}.", m_Name, name);

		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform vec2 named {1}.", m_Name, name);

		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform vec3 named {1}.", m_Name, name);

		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform vec4 named {1}.", m_Name, name);

		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform mat3 named {1}.", m_Name, name);

		// count means how many matrices are we giving, so we put 1. If we used DirectX maths (column-major order) we would need to say GL_TRUE for auto transpose to OpenGL maths (row-major order).
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(matrix)); // f means float and v means an array as it is 16 floats
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		const auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			RS_CORE_ERROR("{0} shader unable to locate uniform mat4 named {1}.", m_Name, name);

		// count means how many matrices are we giving, so we put 1. If we used DirectX maths (column-major order) we would need to say GL_TRUE for auto transpose to OpenGL maths (row-major order).
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)); // f means float and v means an array as it is 16 floats
	}

}
