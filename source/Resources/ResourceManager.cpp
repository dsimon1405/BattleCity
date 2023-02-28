#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t slashIndex = executablePath.find_last_of("/\\");
	mapPath = executablePath.substr(0, slashIndex);
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

	std::shared_ptr<Renderer::ShaderProgram> newShader = mapShaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (!newShader->IsCompiled())
	{
		std::cerr << "Can't load shader program:\nVertex: " << vertexPath << "\nFragment: " << fragmentPath << std::endl;
		return nullptr;
	}

	return newShader;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::GetShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator pShaderProgram = mapShaderPrograms.find(shaderName);
	if (pShaderProgram == mapShaderPrograms.end())
	{
		std::cerr << "Can't find the shader program: " << shaderName << std::endl;
		return nullptr;
	}

	return pShaderProgram->second;
}

std::string ResourceManager::GetFileString(const std::string& relativeFilePath) const
{
	std::ifstream file;
	file.open(mapPath + "\\" + relativeFilePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Faild to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
