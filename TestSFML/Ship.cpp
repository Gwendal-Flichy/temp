#include "Ship.h"

#include "Game.h"
#include "Fireball.h"

Vec2 getShipSize()
{
    return { 128.f, 128.f };
}

Ship::Ship(IGameObjectContainer& game, const Vec2& position)
  : IGameObject(game)
  , m_PV(2)
  , m_angle(0.f)
  , m_position(position)
  , m_isDead(false)
{
    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("Truc.bmp"));
}

void Ship::handleInputs(const sf::Event& event)
{

}

void Ship::update(float deltaTime)
{
    m_angle += 0.01f;
}

void Ship::render(sf::RenderWindow& window)
{
    m_sprite.setRotation(m_angle / 3.14159265f * 180.f);
    m_sprite.setOrigin(getShipSize().x / 2.f, getShipSize().y / 2.f);
    m_sprite.setPosition(m_position.x, m_position.y);
    window.draw(m_sprite);
}

AABB Ship::getBoundingBox() const
{
    return { m_position - getShipSize() / 2.f, m_position + getShipSize() / 2.f };
}

GameObjectType Ship::gameObjectType()
{
    return SHIP_TYPE;
}

void Ship::takeDamage(int dmg)
{
    m_PV -= dmg;
    if (m_PV <= 0)
        die();
}

void Ship::die()
{
    if (m_isDead)
        return;

    m_isDead = true;
    const int nbFireball = 16;
    const float speed = 36.f;
    
    float dAngle = 2.0f * 3.1415926535f / static_cast<float>(nbFireball);

    for (int i = 0; i < nbFireball; ++i)
    {
        float cAngle = dAngle * static_cast<float>(i);
        new Fireball(m_owner, m_position, Vec2{ speed * std::cos(cAngle) , speed * std::sin(cAngle) });
    }

    destroy();
}
