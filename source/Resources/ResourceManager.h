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
	class AnimatedSprite;
}

class ResourceManager
{
public:
	static void SetExecutablePath(const std::string& executablePath);
	static void UnloadAllResources();
	
	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	static std::shared_ptr<Renderer::ShaderProgram> LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<Renderer::ShaderProgram> GetShaderProgram(const std::string& shaderName);

	static std::shared_ptr<Renderer::Texture2D> LoadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<Renderer::Texture2D> GetTexture(const std::string& textureName);

	static std::shared_ptr<Renderer::Sprite> LoadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName,
			const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "default");
	static std::shared_ptr<Renderer::Sprite> GetSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::AnimatedSprite> LoadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName,
		const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName = "default");
	static std::shared_ptr<Renderer::AnimatedSprite> GetAnimatedSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::Texture2D> LoadTextureAtlas(std::string textureName, std::string texturePath, std::vector<std::string> subTextures, const unsigned int subTextureWidth, const unsigned int subTextureWHeight);

private:
	using ShaderProgramsMap = std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>>;
	static ShaderProgramsMap m_shaderPrograms;

	using TexturesMap = std::map<const std::string, std::shared_ptr<Renderer::Texture2D>>;
	static TexturesMap m_textures;

	using SpritesMap = std::map<const std::string, std::shared_ptr<Renderer::Sprite>>;
	static SpritesMap m_sprites;

	using AnimatedSpritesMap = std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>>;
	static AnimatedSpritesMap m_animatedSprites;

	static std::string m_path;

	static  std::string GetFileString(const std::string& relativeFilePath);
};