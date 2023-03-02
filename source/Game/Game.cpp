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
    auto pDefaultShaderProgram = ResourceManager::LoadShaders("DefaultShader", "resources\\shaders\\vertex.txt", "resources\\shaders\\fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program!" << "DefaultShader" << std::endl;
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::LoadShaders("SpriteShader", "resources\\shaders\\vSprite.txt", "resources\\shaders\\fSprite.txt");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't create shader program!" << "SpriteShader" << std::endl;
        return false;
    }

    auto tex = ResourceManager::LoadTexture("DefaultTexture", "resources\\textures\\map_16x16.png");

    std::vector<std::string> subTexturesNames =
    {
        "block",
        "topBlock",
        "bottomBlock",
        "leftBlock",
        "rightBlock",
        "topLeftBlock",
        "topRightBlock",
        "bottomLeftBlock",

        "bottomRightBlock",
        "beton",
        "topBeton",
        "bottomBeton",
        "leftBetom",
        "rightBetom",
        "topLeftBetom",
        "topRightBetom",

        "bottomLeftBetom",
        "bottomRightBetom",
        "water1",
        "water2",
        "water3",
        "trees",
        "ice",
        "wall",

        "eagle",
        "deadEagle",
        "nothing",
        "respawn1",
        "respawn2",
        "respawn3",
        "respawn4",
    };

    auto pTextureAtlas = ResourceManager::LoadTextureAtlas("DefaultTextureAtlas", "resources\\textures\\map_16x16.png", std::move(subTexturesNames), 16, 16);

    auto pAnimatedSprite = ResourceManager::LoadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
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

    pDefaultShaderProgram->Use();
    pDefaultShaderProgram->SetInt("tex", 0);

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(50.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 200.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pDefaultShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->Use();
    pSpriteShaderProgram->SetInt("tex", 0);
    pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);



    std::vector<std::string> tanksSubTexturesNames =
    {
        "tankTop1",
        "tankTop2",
        "tankLeft1",
        "tankLeft2",
        "tankBottom1",
        "tankBottom2",
        "tankRight1",
        "tankRight2",
    };

    auto pTanksTextureAtlas = ResourceManager::LoadTextureAtlas("TanksTextureAtlas", "resources\\textures\\tanks.png", std::move(tanksSubTexturesNames), 16, 16);
    auto pTanksAnimatedSprite = ResourceManager::LoadAnimatedSprite("TanksAnimatedSprite", "TanksTextureAtlas", "SpriteShader", 100, 100, "tankTop1");

    std::vector<std::pair<std::string, uint64_t>> tankTopState;
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop1", 500000000));
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankBottomState;
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom1", 500000000));
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankRightState;
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight1", 500000000));
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankLeftState;
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft1", 500000000));
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft2", 500000000));

    pTanksAnimatedSprite->InsertState("tankTopState", std::move(tankTopState));
    pTanksAnimatedSprite->InsertState("tankBottomState", std::move(tankBottomState));
    pTanksAnimatedSprite->InsertState("tankLeftState", std::move(tankLeftState));
    pTanksAnimatedSprite->InsertState("tankRightState", std::move(tankRightState));

    pTanksAnimatedSprite->SetState("tankTopState");

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));


    return true;
}
