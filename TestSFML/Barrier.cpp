#include "Barrier.h"

#include "Game.h"

Barrier::Barrier(IGameObjectContainer& game, const AABB& bbox) : IGameObject(game), m_bbox(bbox)
{ }

void Barrier::handleInputs(const sf::Event& event)
{ }

void Barrier::render(sf::RenderWindow& window)
{}

void Barrier::update(float deltaTime)
{
}

AABB Barrier::getBoundingBox() const
{
    return m_bbox;
}

GameObjectType Barrier::gameObjectType()
{
    return BARRIER_TYPE;
}
