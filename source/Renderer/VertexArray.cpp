#include "VertexArray.h"

namespace RenderEngine
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexBuffer) noexcept
	{
		m_id = vertexBuffer.m_id;
		vertexBuffer.m_id = 0;
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertexBuffer) noexcept
	{
		m_id = vertexBuffer.m_id;
		vertexBuffer.m_id = 0;
	}

	void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
	{
		Bind();
		vertexBuffer.Bind();
		const auto layoutElements = layout.GetLayoutElements();
		GLbyte* offset = nullptr;
		for (unsigned int i = 0; i < layoutElements.size(); i++)
		{
			const auto& currentLayoutElement = layoutElements[i];
			GLuint currentAttribIndex = m_elementCount + i;
			glEnableVertexAttribArray(m_elementCount);
			glVertexAttribPointer(currentAttribIndex, currentLayoutElement.count, currentLayoutElement.type, currentLayoutElement.normalized, layout.GetStride(), offset);
			offset += currentLayoutElement.size;
		}
		m_elementCount += static_cast<unsigned int>(layoutElements.size());
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}
