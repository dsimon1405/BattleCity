#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{
	Renderer::Sprite::Sprite(std::shared_ptr<Texture2D> pTexture,
		std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotation)
		: m_pTexture(std::move(pTexture)),
		m_pShaderProgram(std::move(pShaderProgram)),
		m_position(position),
		m_size(size),
		m_rotation(rotation)
	{
		// 2--3    1
		// | /	 / |
		// 1	3--2
		const GLfloat vertexCoords[] =
		{
			//   X    Y
				0.f, 0.f,
				0.f, 1.f,
				1.f, 1.f,

				1.f, 1.f,
				1.f, 0.f,
				0.f, 0.f
		};

		const GLfloat textureCoords[] =
		{
			//   U    V
				0.f, 0.f,
				0.f, 1.f,
				1.f, 1.f,

				1.f, 1.f,
				1.f, 0.f,
				0.f, 0.f
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_vertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &m_textureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// освобождаем буферы
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Renderer::Sprite::~Sprite()
	{
		// особождается память видеокарты
		glDeleteBuffers(1, &m_vertexCoordsVBO);
		glDeleteBuffers(1, &m_textureCoordsVBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Renderer::Sprite::Render() const
	{
		m_pShaderProgram->Use();

		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(m_position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
		model = glm::scale(model, glm::vec3(m_size, 1.f));

		glBindVertexArray(m_VAO);
		m_pShaderProgram->SetMatrix4("modelMat", model);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Renderer::Sprite::SetPosition(const glm::vec2& position)
	{
		m_position = position;
	}

	void Renderer::Sprite::SetSize(const glm::vec2& size)
	{
		m_size = size;
	}

	void Renderer::Sprite::SetRotation(const float rotation)
	{
		m_rotation = rotation;
	}
}