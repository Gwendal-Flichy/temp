#pragma once

#include "IGameObject.h"
#include "Tree.h"
#include <SFML/Graphics.hpp>

#include "PlayerShip.h"

class IGameObjectContainer;

class Boss : public IGameObject
{
public:
    Boss(IGameObjectContainer& game, const Vec2& position);

    void handleInputs(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    AABB getBoundingBox() const override;
    GameObjectType gameObjectType() override;


    void fireWithPistol();
    void fireWithShotgun();
    

    bool findValidTarget();
    bool isCurrentTargetValid() const;
    void setSpeed(const int& Speed);
    float calculateAngleToTarget() const;

    Vec2 getPosition() const;
    Vec2 getDistanceToPlayer() const;
private:
    int m_speed;
    float m_angle;

    sf::Sprite m_sprite;
    BT::RootNode m_rootNode;
    Vec2 m_position;
    Vec2 m_velocity;
    std::vector<IGameObject*> m_allGameObjects;
    PlayerShip* m_currentTarget;
};

