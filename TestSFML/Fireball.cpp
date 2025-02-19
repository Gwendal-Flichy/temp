#include "Fireball.h"

#include "Game.h"

Vec2 getFireballSize()
{
    return { 16.f, 16.f };
}

Fireball::Fireball(IGameObjectContainer& game, const Vec2& position, const Vec2& velocity)
    : IGameObject(game)
    , m_position(position)
    , m_velocity(velocity)
{
    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("Fireball.bmp"));
}

void Fireball::handleInputs(const sf::Event& event)
{

}

void Fireball::update(float deltaTime)
{
    float dt = 1.f / 60.f;
    m_position += m_velocity * dt;
}

void Fireball::render(sf::RenderWindow& window)
{
    m_sprite.setOrigin(getFireballSize().x / 2.f, getFireballSize().y / 2.f);
    m_sprite.setPosition(m_position.x, m_position.y);
    window.draw(m_sprite);
}

AABB Fireball::getBoundingBox() const
{
    return { m_position - getFireballSize() / 2.f, m_position + getFireballSize() / 2.f };
}

GameObjectType Fireball::gameObjectType()
{
    return FIREBALL_TYPE;
}
