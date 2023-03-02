#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
}

class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> GetShaderProgram(const std::string& shaderName);

	std::shared_ptr<Renderer::Texture2D> LoadTexture(const std::string& textureName, const std::string& texturePath);
	std::shared_ptr<Renderer::Texture2D> GetTexture(const std::string& textureName);

	std::shared_ptr<Renderer::Sprite> LoadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName,
			const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "default");
	std::shared_ptr<Renderer::Sprite> GetSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::Texture2D> LoadTextureAtlas(const std::string& textureName, const std::string& texturePath, const std::vector<std::string> subTextures, const unsigned int subTextureWidth, const unsigned int subTextureWHeight);

private:
	using ShaderProgramsMap = std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>>;
	ShaderProgramsMap m_shaderPrograms;

	using TexturesMap = std::map<const std::string, std::shared_ptr<Renderer::Texture2D>>;
	TexturesMap m_textures;

	using SpritesMap = std::map<const std::string, std::shared_ptr<Renderer::Sprite>>;
	SpritesMap m_sprites;

	std::string mapPath;

	std::string GetFileString(const std::string& relativeFilePath) const;
};