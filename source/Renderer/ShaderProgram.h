#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram(ShaderProgram&& shaderProgram);
		ShaderProgram& operator=(ShaderProgram&& shaderProgram);
		~ShaderProgram();
		bool IsCompiled() const { return program_isCompiled; }
		void Use() const;

		// запрещаем конструкторы
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

	private:
		bool program_isCompiled = false;
		GLuint programID = 0;
		bool CreateShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
	};
}