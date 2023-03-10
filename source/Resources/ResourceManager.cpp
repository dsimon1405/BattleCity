#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;
std::vector<std::vector<std::string>> ResourceManager::m_levels;

void ResourceManager::SetExecutablePath(const std::string& executablePath)
{
	RenderEngine::Sprite d();
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

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
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

	std::shared_ptr<RenderEngine::ShaderProgram> newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (!newShader->IsCompiled())
	{
		std::cerr << "Can't load shader program:\nVertex: " << vertexPath << "\nFragment: " << fragmentPath << std::endl;
		return nullptr;
	}

	return newShader;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::GetShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it == m_shaderPrograms.end())
	{
		std::cerr << "Can't find the shader program: " << shaderName << std::endl;
		return nullptr;
	}

	return it->second;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::LoadTexture(const std::string& textureName, const std::string& texturePath)
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

	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::GetTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the texture: " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::LoadSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName)
{
	auto pTexture = GetTexture(textureName);
	if (!pTexture) std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;

	auto pShader = GetShaderProgram(shaderName);
	if (!pTexture) std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;

	std::shared_ptr<RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName, std::make_shared<RenderEngine::Sprite>(pTexture, subTextureName, pShader)).first->second;

	return newSprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::GetSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::LoadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName)
{
	auto pTexture = GetTexture(textureName);
	if (!pTexture) std::cerr << "Can't find the texture: " << textureName << "for the sprite: " << spriteName << std::endl;

	auto pShader = GetShaderProgram(shaderName);
	if (!pTexture) std::cerr << "Can't find the shader program: " << shaderName << "for the sprite: " << spriteName << std::endl;

	std::shared_ptr<RenderEngine::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName, std::make_shared<RenderEngine::AnimatedSprite>(pTexture, subTextureName, pShader)).first->second;

	return newSprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::GetAnimatedSprite(const std::string& spriteName)
{
	auto it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find animated sprite: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::LoadTextureAtlas(std::string textureName, std::string texturePath, std::vector<std::string> subTextures, const unsigned int subTextureWidth, const unsigned int subTextureWHeight)
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
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureWHeight + 0.01f) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);
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

bool ResourceManager::loadJSONResourses(const std::string& JSONPath)
{
	const std::string JSONString = GetFileString(JSONPath);
	if (JSONString.empty())
	{
		std::cerr << "No JSON resources file!" << std::endl;
		return false;
	}

	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult)
	{
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "No JSON file: " << JSONPath << std::endl;
		return false;
	}

	auto shadersIt = document.FindMember("shaders");
	if (shadersIt != document.MemberEnd())
	{
		for (const auto& it : shadersIt->value.GetArray())
		{
			const std::string name = it["name"].GetString();
			const std::string filePath_v = it["filePath_v"].GetString();
			const std::string filePath_f = it["filePath_f"].GetString();
			LoadShaders(name, filePath_v, filePath_f);
		}
	}

	auto textureAtlasesIt = document.FindMember("textureAtlases");
	if (textureAtlasesIt != document.MemberEnd())
	{
		for (const auto& it : textureAtlasesIt->value.GetArray())
		{
			const std::string name = it["name"].GetString();
			const std::string filePath = it["filePath"].GetString();
			const unsigned int subTextureWidth = it["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = it["subTextureHeight"].GetUint();

			auto subTexturesArray = it["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());
			for (const auto& it : subTexturesArray)
			{
				subTextures.emplace_back(it.GetString());
			}
			LoadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
		}
	}

	auto spritesIt = document.FindMember("sprites");
	if (spritesIt != document.MemberEnd())
	{
		for (const auto& it : spritesIt->value.GetArray())
		{
			const std::string name = it["name"].GetString();
			const std::string textureAtlas = it["textureAtlas"].GetString();
			const std::string shader = it["shader"].GetString();
			const std::string subTextureName = it["subTextureName"].GetString();

			LoadSprite(name, textureAtlas, shader, subTextureName);
		}
	}

	auto animatedSpritesIt = document.FindMember("animatedSprites");
	if (animatedSpritesIt != document.MemberEnd())
	{
		for (const auto& it : animatedSpritesIt->value.GetArray())
		{
			const std::string name = it["name"].GetString();
			const std::string textureAtlas = it["textureAtlas"].GetString();
			const std::string shader = it["shader"].GetString();
			const std::string initialSubTexture = it["initialSubTexture"].GetString();

			auto pAnimatedSprite = LoadAnimatedSprite(name, textureAtlas, shader, initialSubTexture);
			if (!pAnimatedSprite) continue;

			auto statesArray = it["states"].GetArray();
			for (const auto& state : statesArray)
			{
				const std::string stateName = state["stateName"].GetString();

				auto framesArray = state["frames"].GetArray();
				std::vector<std::pair<std::string, uint64_t>> frameState;
				frameState.reserve(framesArray.Size());
				for (const auto& frame : framesArray)
				{
					std::string subTexture = frame["subTexture"].GetString();
					uint64_t duration = frame["duration"].GetUint64();

					frameState.emplace_back(std::pair<std::string, uint64_t>(subTexture, duration));

				}
				pAnimatedSprite->InsertState(stateName, std::move(frameState));
			}
		}
	}

	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd())
	{
		for (const auto& it : levelsIt->value.GetArray())
		{
			auto description = it["description"].GetArray();
			std::vector<std::string> levelRows;
			levelRows.reserve(description.Size());
			size_t maxLength = 0;
			for (const auto& it : description)
			{
				levelRows.emplace_back(it.GetString());
				if (maxLength < levelRows.back().length());
				{
					maxLength = levelRows.back().length();
				}
			}

			for (auto& it : levelRows)
			{
				while (it.length() < maxLength)
				{
					it.append("D");
				}
			}
			m_levels.emplace_back(std::move(levelRows));
		}
	}

	return true;
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
