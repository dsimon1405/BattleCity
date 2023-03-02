#pragma once

#include <glm/vec2.hpp>
#include <memory>

namespace RenderEngine
{
	class AnimatedSprite;
}

class Tank
{
public:
	enum class EOrientation { Top, Bottom, Left, Right };

	Tank(std::shared_ptr<RenderEngine::AnimatedSprite>	pSprite, const float velocity, const::glm::vec2& position);
	
	void Render() const;
	void SetOrintation(const EOrientation eOrientation);
	void Move(const bool move);
	void Update(const uint64_t delta);

private:
	EOrientation m_eOriebtation;
	std::shared_ptr<RenderEngine::AnimatedSprite> m_pSprite;
	bool m_move;
	float m_velocity;
	glm::vec2 m_position;
	glm::vec2 m_moveOffset;
};