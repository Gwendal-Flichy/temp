// Melee.cpp - Version complètement révisée
#include "melee.h"
#include "Game.h"
#include "PlayerShip.h"
#include "Boss.h"

// Constantes pour faciliter la conversion et les comparaisons
const float PI = 3.14159265f;
const float TWO_PI = 2.0f * PI;

// Fonction utilitaire pour normaliser un angle entre 0 et 2π
float normalizeAngle(float angle) {
    while (angle >= TWO_PI)
        angle -= TWO_PI;
    while (angle < 0.0f)
        angle += TWO_PI;
    return angle;
}

// Fonction pour convertir les degrés en radians
float degreesToRadians(float degrees) {
    return degrees * PI / 180.0f;
}

Vec2 getOrbitalProjectileSize()
{
    return { 16.f, 64.f };
}

Melee::Melee(IGameObjectContainer& game, IGameObject* owner,
    float initialAngle, float orbitalRadius, float angularSpeed, float totalAngleToDestroy)
    : IGameObject(game)
    , m_owner(owner)
    , m_orbitalRadius(orbitalRadius)
    , m_angle(normalizeAngle(initialAngle))  // Normaliser dès le début
    , m_initialAngle(normalizeAngle(initialAngle))
    , m_angularSpeed(angularSpeed)
    , m_velocity(0.f, 0.f)
    , m_position(0.f, 0.f)
    , m_hasEndAngle(totalAngleToDestroy > 0.0f)
    , m_totalRotation(0.f)  // Nouveau compteur pour la rotation totale
	, m_totalAngleToDestroy(totalAngleToDestroy)
{
    // Charger la texture
    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("Fireball.bmp"));

    // Calculer l'angle de fin
    if (m_hasEndAngle)
    {
        // Convertir en radians si la valeur semble être en degrés
        if (m_totalAngleToDestroy > TWO_PI * 10) // Probablement en degrés
            m_totalAngleToDestroy = degreesToRadians(m_totalAngleToDestroy);

        m_endAngle = normalizeAngle(m_initialAngle + m_totalAngleToDestroy);
    }
    else
    {
        m_endAngle = -1.0f;
    }

    // Position initiale
    Vec2 ownerPos = getOwnerPosition();
    m_position.x = ownerPos.x + m_orbitalRadius * std::cos(m_angle);
    m_position.y = ownerPos.y + m_orbitalRadius * std::sin(m_angle);
}

void Melee::handleInputs(const sf::Event& event)
{
    // Rien à faire ici
}

void Melee::update(float deltaTime)
{
    // Obtenir la position du propriétaire
    Vec2 ownerPos = getOwnerPosition();

    // Sauvegarder l'angle précédent
    float previousAngle = m_angle;

    // Mettre à jour l'angle
    m_angle += m_angularSpeed * deltaTime;
    m_angle = normalizeAngle(m_angle);

    // Calculer le changement d'angle (en tenant compte des passages par 0/2π)
    float deltaAngle = m_angle - previousAngle;
    if (deltaAngle < -PI) // Passage de 0 à 2π
        deltaAngle += TWO_PI;
    else if (deltaAngle > PI) // Passage de 2π à 0
        deltaAngle -= TWO_PI;

    // Accumuler la rotation totale
    m_totalRotation += std::abs(deltaAngle);

    // Vérifier si on a dépassé l'angle cible ou la rotation totale
    bool shouldDestroy = false;

    if (m_hasEndAngle)
    {
        // Approche 1: Vérifier si la rotation totale a dépassé l'angle demandé
        if (m_totalRotation >= std::abs(m_totalAngleToDestroy))
        {
            shouldDestroy = true;
        }

        // Approche 2: Vérifier si on a passé l'angle de fin spécifique
        if (m_angularSpeed > 0) // Rotation horaire
        {
            if ((previousAngle < m_endAngle && m_angle >= m_endAngle) ||
                (m_endAngle < m_initialAngle && m_angle >= m_endAngle && previousAngle < m_endAngle))
            {
                shouldDestroy = true;
            }
        }
        else // Rotation anti-horaire
        {
            if ((previousAngle > m_endAngle && m_angle <= m_endAngle) ||
                (m_endAngle > m_initialAngle && m_angle <= m_endAngle && previousAngle > m_endAngle))
            {
                shouldDestroy = true;
            }
        }

        if (shouldDestroy)
        {
            destroy();
            return;
        }
    }

    // Calculer la nouvelle position orbitale
    m_position.x = ownerPos.x + m_orbitalRadius * std::cos(m_angle);
    m_position.y = ownerPos.y + m_orbitalRadius * std::sin(m_angle);
}

void Melee::render(sf::RenderWindow& window)
{
    m_sprite.setOrigin(getOrbitalProjectileSize().x / 2.f, getOrbitalProjectileSize().y / 2.f);
    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setRotation(m_angle * 180.f / PI); // Convertir en degrés pour la rotation du sprite
    window.draw(m_sprite);
}

AABB Melee::getBoundingBox() const
{
    return {
        m_position - getOrbitalProjectileSize() / 2.f,
        m_position + getOrbitalProjectileSize() / 2.f
    };
}

GameObjectType Melee::gameObjectType()
{
    return FIREBALL_TYPE;
}

Vec2 Melee::getOwnerPosition() const
{
    if (!m_owner)
        return { 0.f, 0.f };

    switch (m_owner->gameObjectType())
    {
    case PLAYERSHIP_TYPE:
        return static_cast<PlayerShip*>(m_owner)->getPositon();
    case ENEMY_TYPE:
        return static_cast<Boss*>(m_owner)->getPosition();
    default:
        AABB bbox = m_owner->getBoundingBox();
        return (bbox.min + bbox.max) / 2.0f;
    }
}