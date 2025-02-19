#include "Game.h"

#include "IGameObject.h"
#include "Ship.h"
#include "Enemy.h"
#include "Barrier.h"
#include "Fireball.h"
#include "PlayerShip.h"

#define HANDLE_COLLIDE(Type1, Type2, EnumType1, EnumType2, fn) \
if (go1->gameObjectType() == EnumType1 && go2->gameObjectType() == EnumType2)\
{\
    fn(static_cast<Type1*>(go1), static_cast<Type2*>(go2));\
}\
if (go1->gameObjectType() == EnumType2 && go2->gameObjectType() == EnumType1)\
{\
    fn(static_cast<Type1*>(go2), static_cast<Type2*>(go1));\
}

void collision_Ship_Enemy(Ship* go1, Enemy* go2)
{
    go1->takeDamage();
    go2->destroy();
}

void collision_Ship_Fireball(Ship* go1, Fireball* go2)
{
    go1->takeDamage(2);
    go2->destroy();
}

void collision_Barrier_Fireball(Barrier* go1, Fireball* go2)
{
    go2->destroy();
}

void collision_Barrier_PlayerShip(Barrier* go1, PlayerShip* go2)
{
    go2->destroy();
}

void Game::onCollision(IGameObject* go1, IGameObject* go2)
{
    HANDLE_COLLIDE(Ship, Enemy, SHIP_TYPE, ENEMY_TYPE, collision_Ship_Enemy)
    HANDLE_COLLIDE(Ship, Fireball, SHIP_TYPE, FIREBALL_TYPE, collision_Ship_Fireball)
    HANDLE_COLLIDE(Barrier, Fireball, BARRIER_TYPE, FIREBALL_TYPE, collision_Barrier_Fireball)
    HANDLE_COLLIDE(Barrier, PlayerShip, BARRIER_TYPE, PLAYERSHIP_TYPE, collision_Barrier_PlayerShip)
}
