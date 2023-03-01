#include "Texture2D.h"

namespace Renderer
{

	Texture2D::Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels, const GLenum filter, const GLenum wrapMode) : memoryWidth(width), memoryHeight(height)
	{
		switch (channels)
		{
		case 3:
			memoryMode = GL_RGB;
			break;
		default:
			memoryMode = GL_RGBA;
			break;
		}

		glGenTextures(1, &memoryID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, memoryID);
		glTexImage2D(GL_TEXTURE_2D, 0, memoryMode, memoryWidth, memoryHeight, 0, memoryMode, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture2D& Texture2D::operator=(Texture2D&& texture2D) noexcept
	{
		glDeleteTextures(1, &memoryID);
		memoryID = texture2D.memoryID;
		texture2D.memoryID = 0;
		memoryMode = texture2D.memoryMode;
		memoryHeight = texture2D.memoryHeight;
		memoryWidth = texture2D.memoryWidth;
		return *this;
	}

	Texture2D::Texture2D(Texture2D&& texture2D) noexcept
	{
		memoryID = texture2D.memoryID;
		texture2D.memoryID = 0;
		memoryMode = texture2D.memoryMode;
		memoryHeight = texture2D.memoryHeight;
		memoryWidth = texture2D.memoryWidth;
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &memoryID);
	}

	void Texture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, memoryID);
	}
}