#pragma once

#include "IGameObject.h"
#include <SFML/Graphics.hpp>

class IGameObject;

class Melee : public IGameObject
{
public:
    Melee(IGameObjectContainer& game, IGameObject* owner,
        float initialAngle, float orbitalRadius, float angularSpeed,
        float totalAngleToDestroy = -1.0f);

    void handleInputs(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    AABB getBoundingBox() const override;
    GameObjectType gameObjectType() override;

private:
    sf::Sprite m_sprite;
    IGameObject* m_owner;
    Vec2 m_position;
    Vec2 m_velocity;

    // Paramètres orbitaux
    float m_orbitalRadius;
    float m_angle;           // Angle actuel
    float m_initialAngle;    // Angle initial
    float m_angularSpeed;    // Vitesse angulaire
    float m_endAngle;        // Angle de fin pour la destruction
    bool m_hasEndAngle;      // Si true, a un angle de fin
    float m_totalRotation;   // Compteur de rotation totale
    float m_totalAngleToDestroy; // Angle de rotation total avant destruction

    // Méthode helper
    Vec2 getOwnerPosition() const;
};