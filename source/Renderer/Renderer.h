#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

#include <string>

namespace RenderEngine
{
	class Renderer
	{
	public:
		static void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shaderProgram);
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetViewPort(unsigned int width, unsigned int height, unsigned int lefOffset = 0, unsigned int bottomOffset = 0);
		static std::string GetRendererStr();
		static std::string GetVersionStr();
	};
}