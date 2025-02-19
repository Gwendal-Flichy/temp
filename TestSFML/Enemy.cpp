#include "Enemy.h"

#include "Game.h"

Vec2 getEnemySize()
{
    return { 64.f, 64.f };
}

Enemy::Enemy(IGameObjectContainer& game, const Vec2& position, const Vec2& velocity)
    : IGameObject(game)
    , m_position(position)
    , m_velocity(velocity)
{
    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("Enemy.bmp"));
}

void Enemy::handleInputs(const sf::Event& event)
{

}

void Enemy::update(float deltaTime)
{
    float dt = 1.f / 60.f;
    m_position += m_velocity * dt;
}

void Enemy::render(sf::RenderWindow& window)
{
    m_sprite.setOrigin(getEnemySize().x / 2.f, getEnemySize().y / 2.f);
    m_sprite.setPosition(m_position.x, m_position.y);
    window.draw(m_sprite);
}

AABB Enemy::getBoundingBox() const
{
    return { m_position - getEnemySize() / 2.f, m_position + getEnemySize() / 2.f };
}

GameObjectType Enemy::gameObjectType()
{
    return ENEMY_TYPE;
}
