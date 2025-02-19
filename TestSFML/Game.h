#pragma once

#include "TextureCache.h"
#include "IGameObject.h"

#include <SFML/Graphics.hpp>

class IGameObject;

#define private_members private
#define private_function private
#define private_friend_function(Cls) private

class Game : public IGameObjectContainer
{
public:
    friend class IGameObject;

    Game(const std::string& execFilePath);
    ~Game();

    void handleInputs();

    void update();

    void render();

    void run();

    TextureCache& getTextureCache();
    
    virtual Game& getGame() override { return *this; }

private_function:
    void detectCollision();
    void onCollision(IGameObject* go1, IGameObject* go2);
    void renderBoundingBox();

private_members:
    sf::RenderWindow m_window;
    TextureCache m_textureCache;
};
