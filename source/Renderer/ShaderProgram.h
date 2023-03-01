#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>

namespace Renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		~ShaderProgram();
		bool IsCompiled() const { return program_isCompiled; }
		void Use() const;
		void SetInt(const std::string& name, const GLint value);
		void SetMatrix4(const std::string& name, const glm::mat4& matrix);

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

	private:
		bool program_isCompiled = false;
		GLuint programID = 0;
		bool CreateShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
	};
}