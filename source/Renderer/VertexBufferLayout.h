#pragma once

#include <vector>
#include <glad/glad.h>

namespace RenderEngine
{
	struct VertexBufferLayoutElement
	{
		GLuint count;
		GLenum type;
		GLboolean normalized;
		unsigned int size;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();

		void ReserveElements(const size_t count);
		unsigned int GetStride() const { return m_stride; }
		void AddElementLayoutFloat(const unsigned int count, const bool normalized);
		const std::vector<VertexBufferLayoutElement>& GetLayoutElements() const { return m_layoutElement; }

	private:
		std::vector<VertexBufferLayoutElement> m_layoutElement;
		unsigned int m_stride;
	};
}