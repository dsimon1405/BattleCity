#include "Game.h"

#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "Tank.h"

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
    //ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Render();

    if (m_pTank)
    {
        m_pTank->Render();
    }
}

void Game::Update(const uint64_t delta)
{
    //ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Update(delta);


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


    auto pAnimatedSprite = ResourceManager::LoadAnimatedSprite("NewAnimatedSprite", "mapTextureAtlas", "spriteShader", 100, 100, "beton");
    pAnimatedSprite->SetPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> waterState;
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1000000000));
    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

    pAnimatedSprite->InsertState("waterState", std::move(waterState));
    pAnimatedSprite->InsertState("eagleState", std::move(eagleState));

    pAnimatedSprite->SetState("waterState");

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

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));


    return true;
}
