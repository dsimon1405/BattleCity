#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class Tank;
class Level;

class Game
{
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void Render();
	void Update(const uint64_t delta);
	void SetKey(const int key, const int action);
	bool Init();

private:
	std::array<bool, 349> m_keys;

	enum EGameState
	{
		Active, Pause
	};

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::unique_ptr<Tank> m_pTank;
	std::unique_ptr<Level> m_pLevel;
};