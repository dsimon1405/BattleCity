#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;

void ResourceManager::SetExecutablePath(const std::string& executablePath)
{
	size_t slashIndex = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, slashIndex);
}

void ResourceManager::UnloadAllResources()
{
	m_shaderPrograms.clear();
	m_textures.clear();
	m_sprites.clear();
	m_animatedSprites.clear();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = GetFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cout << "No vertex shader" << std::endl;
		return nullptr;
	}
	std::string fragmentString = GetFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cout << "No fragment shader" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram> newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (!newShader->IsCompiled())
	{
		std::cerr << "Can't load shader program:\nVertex: " << vertexPath << "\nFragment: " << fragmentPath << std::endl;
		return nullptr;
	}

	return newShader;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::GetShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it == m_shaderPrograms.end())
	{
		std::cerr << "Can't find the shader program: " << shaderName << std::endl;
		return nullptr;
	}

	return it->second;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::LoadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0,
		width = 0,
		height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "\\" + texturePath).c_str(), &width, &height, &channels, 0);
	if (!pixels)
	{
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::GetTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the texture: " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::LoadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName)
{
	auto pTexture = GetTexture(textureName);
	if (!pTexture) std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;

	auto pShader = GetShaderProgram(shaderName);
	if (!pTexture) std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;

	std::shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(spriteName, std::make_shared<Renderer::Sprite>(pTexture, subTextureName, pShader, glm::vec2(0.f, 0.f), glm::vec2(spriteWidth, spriteHeight))).first->second;

	return newSprite;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::GetSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::LoadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName)
{
	auto pTexture = GetTexture(textureName);
	if (!pTexture) std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;

	auto pShader = GetShaderProgram(shaderName);
	if (!pTexture) std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;

	std::shared_ptr<Renderer::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName, std::make_shared<Renderer::AnimatedSprite>(pTexture, subTextureName, pShader, glm::vec2(0.f, 0.f), glm::vec2(spriteWidth, spriteHeight))).first->second;

	return newSprite;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::GetAnimatedSprite(const std::string& spriteName)
{
	auto it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find animated sprite: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::LoadTextureAtlas(std::string textureName, std::string texturePath, std::vector<std::string> subTextures, const unsigned int subTextureWidth, const unsigned int subTextureWHeight)
{
	auto pTexture = LoadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->Width();
		const unsigned int textureHeight = pTexture->Height();

		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;
		for (auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureWHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
			pTexture->AddSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureWHeight;
			}
		}
	}
	return pTexture;
}

std::string ResourceManager::GetFileString(const std::string& relativeFilePath)
{
	std::ifstream file;
	file.open(m_path + "\\" + relativeFilePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Faild to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
