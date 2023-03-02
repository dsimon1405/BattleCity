#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture,
				   std::string initialSubTexture,
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
		// 1--2
		// |/ |
		// 0--3
		const GLfloat vertexCoords[] =
		{
			//   X    Y
				0.f, 0.f,
				0.f, 1.f,
				1.f, 1.f,
				1.f, 0.f,
		};

		auto subTexture = m_pTexture->GetSubTexture(std::move(initialSubTexture));

		const GLfloat textureCoords[] =
		{
			//				U						V
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
		};

		const GLuint indexes[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		m_vertexCoordsBuffer.Init(vertexCoords, 2 * 4* sizeof(GLfloat));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		m_textureCoordsBuffer.Init(textureCoords, 2 * 4 * sizeof(GLfloat));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		m_indexBuffer.Init(indexes, 6 * sizeof(GLuint));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Sprite::~Sprite()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Renderer::Sprite::Render()
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

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void Sprite::SetPosition(const glm::vec2& position)
	{
		m_position = position;
	}

	void Sprite::SetSize(const glm::vec2& size)
	{
		m_size = size;
	}

	void Sprite::SetRotation(const float rotation)
	{
		m_rotation = rotation;
	}
}