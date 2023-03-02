#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
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
		
		m_vertexCoordsBuffer.Init(vertexCoords, 2 * 4* sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.AddElementLayoutFloat(2, false);
		m_vertexArray.AddBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_textureCoordsBuffer.Init(textureCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.AddElementLayoutFloat(2, false);
		m_vertexArray.AddBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		m_indexBuffer.Init(indexes, 6);

		m_vertexArray.Unbind();
		m_indexBuffer.Unbind();
	}

	Sprite::~Sprite()
	{
	}

	void RenderEngine::Sprite::Render()
	{
		m_pShaderProgram->Use();

		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(m_position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
		model = glm::scale(model, glm::vec3(m_size, 1.f));

		m_pShaderProgram->SetMatrix4("modelMat", model);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->Bind();

		Renderer::Draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
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