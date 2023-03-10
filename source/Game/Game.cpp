#include "Game.h"

#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "GameObjects/Tank.h"
#include "Level.h"

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Game::Game(const glm::ivec2& windowSize)
    : m_eCurrentGameState(EGameState::Active),
    m_windowSize(windowSize)
{
    m_keys.fill(false);
}

Game::~Game()
{
}

void Game::Render()
{
    if (m_pTank)
    {
        m_pTank->Render();
    }

    if (m_pLevel)
    {
        m_pLevel->Render();
    }
}

void Game::Update(const uint64_t delta)
{
    if (m_pLevel)
    {
        m_pLevel->Update(delta);
    }

    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->SetOrintation(Tank::EOrientation::Top);
            m_pTank->Move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pTank->SetOrintation(Tank::EOrientation::Left);
            m_pTank->Move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pTank->SetOrintation(Tank::EOrientation::Right);
            m_pTank->Move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pTank->SetOrintation(Tank::EOrientation::Bottom);
            m_pTank->Move(true);
        }
        else
        {
            m_pTank->Move(false);
        }

        m_pTank->Update(delta);
    }
}

void Game::SetKey(const int key, const int action)
{
    m_keys[key] = action;
}

bool Game::Init()
{
    ResourceManager::loadJSONResourses("resources\\resources.json");

    auto pSpriteShaderProgram = ResourceManager::GetShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: spriteShader" << std::endl;
        return false;
    }

    auto pTextureAtlas = ResourceManager::GetTexture("mapTextureAtlas");
    if (!pTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: mapTextureAtlas" << std::endl;
        return false;
    }

    auto pTanksTextureAtlas = ResourceManager::GetTexture("tanksTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: tanksTextureAtlas" << std::endl;
        return false;
    }

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pSpriteShaderProgram->Use();
    pSpriteShaderProgram->SetInt("tex", 0);
    pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

    auto pTanksAnimatedSprite = ResourceManager::GetAnimatedSprite("tankAnimatedSprite");
    if (!pTanksAnimatedSprite)
    {
        std::cerr << "Can't find texture atlas: tankAnimatedSprite" << std::endl;
        return false;
    }

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(0), glm::vec2(16.f, 16.f));
    m_pLevel = std::make_unique<Level>(ResourceManager::GetLevels()[0]);

    return true;
}
