#include "NPC.h"

#include "NPCBehavior.h"

NPC::NPC(Game& game)
    : m_game(game)
    , m_currentTarget(nullptr)
    , m_ammo(NPC_MAX_AMMO())
    , m_rootNode(this)
{
    auto* behavior = new BT::Sequence (&m_rootNode);

    auto* nTimes = new BT::DoUntilFailure(behavior);

    auto* findEnemy = new BT::FindEnemy(nTimes);
    auto* killAnEnemy = new BT::Retry(nTimes);

    auto* victoryDance = new BT::VictoryDance(behavior);

    auto* ifGunEmpty = new BT::IfGunEmpty(killAnEnemy);
    auto* test = new BT::Sequence(ifGunEmpty);
    new BT::ReloadGun(test);
    new BT::IsEnemyDead(test);
    new BT::Fire(killAnEnemy);
    new BT::IsEnemyDead(killAnEnemy);
}

void NPC::update()
{
    m_rootNode.tick();
}

void NPC::findValidTarget()
{
    for (auto& enemy : m_game.enemies)
    {
        if (enemy.PV > 0)
        {
            m_currentTarget = &enemy;
            return;
        }
    }

    m_currentTarget = nullptr;
}





Enemy::Enemy() : m_rootNode(this)
{

}

void Enemy::update()
{
    m_rootNode.tick();
}






#include <conio.h>

void Game::exec()
{
    while (true)
    {
        npc.update();
        ::_getch();
    }
}
