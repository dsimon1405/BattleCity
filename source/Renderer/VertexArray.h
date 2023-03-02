#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glad/glad.h>

namespace Renderer
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		VertexArray& operator=(VertexArray&& vertexBuffer) noexcept;
		VertexArray(VertexArray&& vertexBuffer) noexcept;

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;

	private:
		GLuint m_id = 0;
		unsigned int m_elementCount = 0;
	};
}