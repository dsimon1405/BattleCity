#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace Renderer
{
	class Texture2D
	{
	public:

		struct SubTexture2D
		{
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightBottomUV) : leftBottomUV(_leftBottomUV), rightTopUV(_rightBottomUV) {}
			SubTexture2D() : leftBottomUV(0.f), rightTopUV(1.f) {}
		};

		Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels = 4, const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		Texture2D& operator=(Texture2D&& texture2D) noexcept;
		Texture2D(Texture2D&& texture2D) noexcept;
		~Texture2D();

		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		void Bind() const;
		void AddSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightBottomUV);
		const SubTexture2D& GetSubTexture(const std::string& name) const
		{
			auto it = m_subTextures.find(name);
			if (it != m_subTextures.end())
			{
				return it->second;
			}
			const static SubTexture2D defaultSubTexture;
			return defaultSubTexture;
		}
		unsigned int Width() const { return m_width; }
		unsigned int Height() const { return m_height; }

	private:
		GLuint memoryID;
		GLuint memoryMode;
		unsigned int m_width;
		unsigned int m_height;

		std::map<std::string, SubTexture2D> m_subTextures;
	};
}