#pragma once

#include <glad/glad.h>

namespace RenderEngine
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();
		IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		void Init(const void* data, const unsigned int count);
		void Bind() const;
		void Unbind() const;
		unsigned int getGount() const { return m_count; }

	private:
		GLuint m_id;
		unsigned int m_count;
	};
}