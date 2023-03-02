#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace RenderEngine
{
	ShaderProgram::ShaderProgram(const std::string& vertexShader,const std::string& fragmentShader)
	{
		GLuint vertexShaderID;
		if (!CreateShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "VERTEX SHADER compile time error" << std::endl;
			return;
		}
		GLuint fragmentShaderID;
		if (!CreateShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
			glDeleteShader(vertexShaderID);		// если фрагментный шейдер не скомпилировался, то удаляем из памяти и вертуксный шейдер
			return;
		}

		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		GLint resultSuccess;
		glGetProgramiv(programID, GL_LINK_STATUS, &resultSuccess);
		if (!resultSuccess)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
			std::cerr << "ERROR::SHADER PROGRAM: Compile time error\n" << infoLog << std::endl;
		}
		else
		{
			program_isCompiled = true;
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		programID = shaderProgram.programID;
		program_isCompiled = shaderProgram.program_isCompiled;

		shaderProgram.programID = 0;
		shaderProgram.program_isCompiled = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(programID);
		programID = shaderProgram.programID;
		program_isCompiled = shaderProgram.program_isCompiled;

		shaderProgram.programID = 0;
		shaderProgram.program_isCompiled = false;
		return *this;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(programID);
	}

	void ShaderProgram::Use() const
	{
		glUseProgram(programID);
	}

	void ShaderProgram::SetInt(const std::string& name, const GLint value)
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
	}

	void ShaderProgram::SetMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	bool ShaderProgram::CreateShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType);
		const char* sourceCode = source.c_str();
		glShaderSource(shaderID, 1, &sourceCode, nullptr);
		glCompileShader(shaderID);
		
		GLint resultSuccess;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &resultSuccess);
		if (!resultSuccess)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::cerr << "ERROR::SHADER: Compile time error\n" << infoLog << std::endl;
			return false;
		}
		return true;
	}
}