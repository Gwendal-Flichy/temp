#pragma once

#include "IGameObject.h"

class Barrier : public IGameObject
{
public:
    Barrier(IGameObjectContainer& game, const AABB& bbox);

    void handleInputs(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    AABB getBoundingBox() const override;
    GameObjectType gameObjectType() override;
    void update(float deltaTime) override;

private:
    AABB m_bbox;
};
