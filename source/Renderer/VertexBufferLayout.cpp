#include "VertexBufferLayout.h"
#pragma once

namespace RenderEngine
{
	VertexBufferLayout::VertexBufferLayout()
		: m_stride(0)
	{}

	void VertexBufferLayout::ReserveElements(const size_t count)
	{
		m_layoutElement.reserve(count);
	}

	void VertexBufferLayout::AddElementLayoutFloat(const unsigned int count, const bool normalized)
	{
		m_layoutElement.push_back({ count, GL_FLOAT, normalized, static_cast<unsigned int>(count * sizeof(GLfloat)) });
		m_stride += m_layoutElement.back().size;
	}
}