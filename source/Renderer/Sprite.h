#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
#include <string>

namespace Renderer
{
	class Texture2D;
	class ShaderProgram;

	class Sprite
	{
	public:
		Sprite(std::shared_ptr<Texture2D> pTexture,
			std::shared_ptr<ShaderProgram> pShaderProgram,
			std::string initialSubTexture,
			const glm::vec2& position = glm::vec2(0.f),
			const glm::vec2& size = glm::vec2(0.f),
			const float rotation = 0.f);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void Render() const;
		void SetPosition(const glm::vec2& position);
		void SetSize(const glm::vec2& size);
		void SetRotation(const float rotation);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		glm::vec2 m_position;
		glm::vec2 m_size;
		float m_rotation;
		GLuint m_VAO;
		GLuint m_vertexCoordsVBO;
		GLuint m_textureCoordsVBO;
	};
}