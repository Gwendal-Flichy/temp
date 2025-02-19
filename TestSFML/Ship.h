#pragma once

#include "IGameObject.h"

#include <SFML/Graphics.hpp>

class IGameObjectContainer;

class Ship : public IGameObject
{
public:
    Ship(IGameObjectContainer& game, const Vec2& position);

    void handleInputs(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    AABB getBoundingBox() const override;
    virtual GameObjectType gameObjectType() override;
    static GameObjectType staticGameObjectType() { return SHIP_TYPE; }
    void takeDamage(int dmg = 1);
    void die();

private:
    sf::Sprite m_sprite;

    int m_PV;
    float m_angle;
    Vec2 m_position;
    bool m_isDead;
};
