#pragma once

#include <glad/glad.h>

namespace RenderEngine
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();
		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void Init(const void* data, const unsigned int size);
		void Update(const void* data, const unsigned int size);
		void Bind() const;
		void Unbind() const;

	private:
		GLuint m_id;
	};
}