#include "PlayerShip.h"

#include "Game.h"
#include "Fireball.h"
#include "melee.h"


// set de valeur qui va bien pour un vaisseau.

//Vec2 getPlayerShipSize() { return { 32.f, 32.f }; }
//float getPlayerShipThrust() { return 400.f; }
//float getPlayerShipRateOfTurn() { return 0.05f; }
//float getPlayerShipFluidFrictionCoef() { return 1.0f; }
//float getPlayerShipMaxVelocity() { return 300.f; }

Vec2 getPlayerShipSize() { return { 32.f, 32.f }; }
float getPlayerShipThrust() { return 2000.f; }
float getPlayerShipRateOfTurn() { return 0.05f; }
float getPlayerShipFluidFrictionCoef() { return 15.0f; }
float getPlayerShipMaxVelocity() { return 300.f; }


PlayerShip::PlayerShip(IGameObjectContainer& game, const Vec2& position)
    : IGameObject(game)
    , m_PV(2)
    , m_angle(0.f)
    , m_position(position)
    , m_velocity(0.f, 0.f)
    , m_isDead(false)
    , m_isAccelerating(false)
{
    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("PlayerShip.bmp"));
    new Melee(m_owner, this, 0.f, 50.f, -1.f, 180.f);
}

void PlayerShip::handleInputs(const sf::Event& event)
{
    //if (event.type == sf::Event::KeyPressed) toggle
    m_isAccelerating = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    m_isTurningLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    m_isTurningRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

void PlayerShip::update(float deltaTime)
{
    
    if (m_isTurningLeft) m_angle -= getPlayerShipRateOfTurn();
    if (m_isTurningRight) m_angle += getPlayerShipRateOfTurn();

    Vec2 acceleration{ 0.f, 0.f };
    if (!m_isAccelerating)
        acceleration = -getPlayerShipFluidFrictionCoef() * m_velocity;

    if (m_isAccelerating)
    {
	    acceleration += getPlayerShipThrust() * Vec2{std::cos(m_angle), std::sin(m_angle) };
        
    }
     
    m_position += m_velocity * deltaTime;
    m_velocity += acceleration * deltaTime;

    if (m_velocity.getLength() > getPlayerShipMaxVelocity())
        m_velocity = m_velocity * (getPlayerShipMaxVelocity() / m_velocity.getLength());
   
}

void PlayerShip::render(sf::RenderWindow& window)
{
    m_sprite.setRotation(m_angle / 3.14159265f * 180.f);
    m_sprite.setOrigin(getPlayerShipSize().x / 2.f, getPlayerShipSize().y / 2.f);
    m_sprite.setPosition(m_position.x, m_position.y);
    window.draw(m_sprite);
}

AABB PlayerShip::getBoundingBox() const
{
    return { m_position - getPlayerShipSize() / 2.f, m_position + getPlayerShipSize() / 2.f };
}

GameObjectType PlayerShip::gameObjectType()
{
    return PLAYERSHIP_TYPE;
}

void PlayerShip::takeDamage(int dmg)
{
    m_PV -= dmg;
    if (m_PV <= 0)
        die();
}

void PlayerShip::die()
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

Vec2 PlayerShip::getPositon() const
{
    return m_position;
}

int PlayerShip::getPV() const
{
    return m_PV;
}

float PlayerShip::getAngle() const
{
    return m_angle;
}
