#include "Renderer.h"

namespace RenderEngine
{
	void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader)
	{
		shader.Use();
		vertexArray.Bind();
		indexBuffer.Bind();

		glDrawElements(GL_TRIANGLES, indexBuffer.getGount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Renderer::SetViewPort(unsigned int width, unsigned int height, unsigned int lefOffset, unsigned int bottomOffset)
	{
		glViewport(lefOffset, bottomOffset, width, height);
	}
	std::string Renderer::GetRendererStr()
	{
		return (char*)glGetString(GL_RENDERER);
	}
	std::string Renderer::GetVersionStr()
	{
		return (char*)glGetString(GL_VERSION);
	}
}
