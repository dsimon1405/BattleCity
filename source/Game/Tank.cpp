#include "Tank.h"

#include "../Renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const::glm::vec2& position)
	: m_eOriebtation(EOrientation::Top),
	m_pSprite(std::move(pSprite)),
	m_move(false),
	m_velocity(velocity),
	m_position(position),
	m_moveOffset(glm::vec2(0.f, 1.f))
{
	m_pSprite->SetPosition(m_position);
}

void Tank::Render() const
{
	m_pSprite->Render();
}

void Tank::SetOrintation(const EOrientation eOrientation)
{
	if (m_eOriebtation == eOrientation)
	{
		return;
	}

	m_eOriebtation = eOrientation;
	switch (m_eOriebtation)
	{
	case Tank::EOrientation::Top:
		m_pSprite->SetState("tankTopState");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Tank::EOrientation::Bottom:
		m_pSprite->SetState("tankBottomState");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;
	case Tank::EOrientation::Left:
		m_pSprite->SetState("tankLeftState");
		m_moveOffset.x = -1.f;
		m_moveOffset.y = 0.f;
		break;
	case Tank::EOrientation::Right:
		m_pSprite->SetState("tankRightState");
		m_moveOffset.x = 1.f;
		m_moveOffset.y = 0.f;
		break;
	default:
		break;
	}
}

void Tank::Move(const bool move)
{
	m_move = move;
}

void Tank::Update(const uint64_t delta)
{
	if (m_move)
	{
		m_position += delta * m_velocity * m_moveOffset;
		m_pSprite->SetPosition(m_position);
		m_pSprite->Update(delta);
	}
}
