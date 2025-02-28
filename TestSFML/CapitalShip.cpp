#include "CapitalShip.h"

#include <iostream>

#include "Game.h"


Vec2 getTurretSize() { return { 32.f, 32.f }; }

Turret::Turret(IGameObjectContainer& game,  const Vec2& position)
    : IGameObject(game.getGame())
    , m_position(position)
	, owner(static_cast<CapitalShip*>(&game))
{



    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("Turret.png"));
    
}

void Turret::handleInputs(const sf::Event& event)
{
    
}

void Turret::update(float deltaTime)
{
    m_position = { owner->m_position.x - 16.f ,owner->m_position.y - 32.f };
    
    
}

void Turret::render(sf::RenderWindow& window)
{
    m_sprite.setOrigin(getTurretSize().x / 2.f, getTurretSize().y / 2.f );
    
    m_sprite.setPosition(m_position.x, m_position.y);
    window.draw(m_sprite);
}

AABB Turret::getBoundingBox() const
{
    return { m_position - getTurretSize() / 2.f, m_position + getTurretSize() / 2.f };
}

GameObjectType Turret::gameObjectType()
{
    return ENEMY_TYPE;
}






Vec2 getCapitalShipSize() { return { 256.f, 128.f }; }
float getCapitalShipThrust() { return 400.f; }
float getCapitalShipRateOfTurn() { return 0.05f; }
float getCapitalShipFluidFrictionCoef() { return 1.0f; }
float getCapitalShipMaxVelocity() { return 300.f; }


CapitalShip::CapitalShip(IGameObjectContainer& game, const Vec2& position)
    : IGameObjectCompound(game)
    , m_PV(2)
    , m_angle(0.f)
    , m_position(position)
    , m_velocity(0.f, 0.f)
    , m_isDead(false)
    , m_isAccelerating(false)
    , m_isTurningLeft(false)
    , m_isTurningRight(false)
{
    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("CapitalShip.png"));
    new Turret(*this, m_position + Vec2(-16.f, -32.f));
}

void CapitalShip::handleInputs(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
        m_isAccelerating = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    m_isTurningLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    m_isTurningRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

void CapitalShip::update(float deltaTime)
{
    if (m_isTurningLeft) m_angle -= getCapitalShipRateOfTurn();
    if (m_isTurningRight) m_angle += getCapitalShipRateOfTurn();

    Vec2 acceleration{ 0.f, 0.f };
    if (!m_isAccelerating)
        acceleration = -getCapitalShipFluidFrictionCoef() * m_velocity;

    if (m_isAccelerating)
        acceleration += getCapitalShipThrust() * Vec2 { std::cos(m_angle), std::sin(m_angle) };

    m_position += m_velocity * deltaTime;
    m_velocity += acceleration * deltaTime;

    if (m_velocity.getLength() > getCapitalShipMaxVelocity())
        m_velocity = m_velocity * (getCapitalShipMaxVelocity() / m_velocity.getLength());
}

void CapitalShip::render(sf::RenderWindow& window)
{
    m_sprite.setRotation(m_angle / 3.14159265f * 180.f);
    m_sprite.setOrigin(getCapitalShipSize().x / 2.f, getCapitalShipSize().y / 2.f);
    m_sprite.setPosition(m_position.x, m_position.y);
    window.draw(m_sprite);
}

AABB CapitalShip::getBoundingBox() const
{
    return { m_position - getCapitalShipSize() / 2.f, m_position + getCapitalShipSize() / 2.f };
}

GameObjectType CapitalShip::gameObjectType()
{
    return CAPITAL_SHIP;
}



void CapitalShip::takeDamage(int dmg)
{
    m_PV -= dmg;
    if (m_PV <= 0)
        die();
}

void CapitalShip::die()
{
    if (m_isDead)
        return;

    m_isDead = true;

    destroy();
}

