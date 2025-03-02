#pragma once

#include "MyMath.h"

#include <vector>

namespace sf { class RenderWindow; }
namespace sf { class Event; }

class Game;

enum GameObjectType
{
    SHIP_TYPE = 0
  , ENEMY_TYPE = 1
  , FIREBALL_TYPE = 2
  , BARRIER_TYPE = 3
  , PLAYERSHIP_TYPE = 4
  , CAPITAL_SHIP = 5
  , TURRET_TYPE = 6
};

class IGameObject;

class IGameObjectContainer
{
public:
    void _addObject(IGameObject*);
    void _removeObject(IGameObject*);
    void _toBeRemoveObject(IGameObject*);
    void _deferedAddObject(IGameObject*);
    void _deferedAddObjects();
    void _cleanObject();
    std::vector<IGameObject*> getAllGameOjects() const;
    ~IGameObjectContainer();

    virtual Game& getGame() = 0;

protected:
    std::vector<IGameObject*> m_allGameObjects;
    std::vector<IGameObject*> m_toBeRemovedGameObjects;
    std::vector<IGameObject*> m_toBeAddedGameObjects;
};

class IGameObject
{
public:
    IGameObject(IGameObjectContainer& owner);
    virtual ~IGameObject();

    virtual void handleInputs(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual AABB getBoundingBox() const = 0;
    
    virtual GameObjectType gameObjectType() = 0;
    void destroy();
    IGameObjectContainer& getOwner() { return m_owner; }
    const IGameObjectContainer& getOwner() const { return m_owner; }

protected:
    IGameObjectContainer& m_owner;
};

class IGameObjectCompound : public IGameObject, public IGameObjectContainer
{
public:
    IGameObjectCompound(IGameObjectContainer& owner);

    void handleInputs(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};
