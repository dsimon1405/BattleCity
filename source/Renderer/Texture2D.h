#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer
{
	class Texture2D
	{
	public:
		Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels, const GLenum filter, const GLenum wrapMode);
		Texture2D& operator=(Texture2D&& texture2D) noexcept;
		Texture2D(Texture2D&& texture2d) noexcept;
		~Texture2D();

		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		void Bind() const;

	private:
		GLuint memoryID;
		GLuint memoryMode;
		unsigned int memoryWidth;
		unsigned int memoryHeight;
	};
}