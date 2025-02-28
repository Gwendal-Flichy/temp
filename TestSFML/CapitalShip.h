#pragma once

#include "IGameObject.h"

#include <SFML/Graphics.hpp>

class IGameObjectContainer;

class CapitalShip;


class Turret : public IGameObject
{
public:
    Turret(IGameObjectContainer& game,  const Vec2& position);

    void handleInputs(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    AABB getBoundingBox() const override;
    GameObjectType gameObjectType() override;

private:
    sf::Sprite m_sprite;
    
    Vec2 m_position;

    CapitalShip* owner;
};

class CapitalShip : public IGameObjectCompound
{
public:
    CapitalShip(IGameObjectContainer& game, const Vec2& position);

    void handleInputs(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    AABB getBoundingBox() const override;
    virtual GameObjectType gameObjectType() override;
    void takeDamage(int dmg = 1);
    void die();
    virtual Game& getGame() override { return this->getOwner().getGame(); }
    
    Vec2 m_position;
    float m_angle;
private:
    sf::Sprite m_sprite;

    //  Inputs
    bool m_isAccelerating;
    bool m_isTurningLeft;
    bool m_isTurningRight;

    //  Gameplay
    int m_PV;
    bool m_isDead;

    //  Physics
    
    
    Vec2 m_velocity;
};


