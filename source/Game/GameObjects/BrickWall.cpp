#include "BrickWall.h"

#include "../../Renderer/Sprite.h"

BrickWall::BrickWall(const std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObject(position, size, rotation),
	m_pCurrentSprite(pSprite)
{
}

void BrickWall::Render() const
{
	m_pCurrentSprite->Render(m_position, m_size, m_rotation);
}

void BrickWall::Update(const uint64_t delta)
{
}
