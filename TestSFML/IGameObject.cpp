#include "IGameObject.h"

#include "Game.h"


IGameObjectContainer::~IGameObjectContainer()
{
    auto allGameObjects = m_allGameObjects;
    for (auto& gameObject : allGameObjects)
        delete gameObject;
}

void IGameObjectContainer::_addObject(IGameObject* go)
{
    m_toBeAddedGameObjects.push_back(go);
}

struct RemoveNonExisitingGameObject : public std::runtime_error
{
    RemoveNonExisitingGameObject() : std::runtime_error("Try to remove non existing Game Object.")
    {
    }
};

void IGameObjectContainer::_removeObject(IGameObject* go)
{
    auto it = std::find(m_allGameObjects.begin(), m_allGameObjects.end(), go);
    if (it == m_allGameObjects.end()) // No elem found.
        throw RemoveNonExisitingGameObject();

    m_allGameObjects.erase(it);
}

void IGameObjectContainer::_toBeRemoveObject(IGameObject* go)
{
    auto it = std::find(m_toBeRemovedGameObjects.begin(), m_toBeRemovedGameObjects.end(), go);

    if (it != m_toBeRemovedGameObjects.end())
        return;

    m_toBeRemovedGameObjects.push_back(go);
}

void IGameObjectContainer::_cleanObject()
{
    for (auto& go : m_toBeRemovedGameObjects)
        delete go;

    m_toBeRemovedGameObjects.clear();
}

void IGameObjectContainer::_deferedAddObject(IGameObject* go)
{
    m_allGameObjects.push_back(go);
}

void IGameObjectContainer::_deferedAddObjects()
{
    for (auto& go : m_toBeAddedGameObjects)
        _deferedAddObject(go);

    m_toBeAddedGameObjects.clear();
}

std::vector<IGameObject*> IGameObjectContainer::getAllGameOjects() const
{
    return m_allGameObjects;
}


IGameObject::IGameObject(IGameObjectContainer& owner) : m_owner(owner)
{
    m_owner._addObject(this);
}

IGameObject::~IGameObject()
{
    m_owner._removeObject(this);
}

void IGameObject::destroy()
{
    m_owner._toBeRemoveObject(this);
}

void IGameObjectCompound::handleInputs(const sf::Event& event)
{
    for (auto& gameObject : m_allGameObjects)
        gameObject->handleInputs(event);
}

void IGameObjectCompound::update(float deltaTime)
{
    for (auto& gameObject : m_allGameObjects)
        gameObject->update(deltaTime);
}

void IGameObjectCompound::render(sf::RenderWindow& window)
{
    for (auto& gameObject : m_allGameObjects)
        gameObject->render(window);
}
IGameObjectCompound::IGameObjectCompound(IGameObjectContainer& owner)
    : IGameObject(owner)
{
}

