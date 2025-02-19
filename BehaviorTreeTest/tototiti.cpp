#include "Tree.h"

#include <conio.h>

void testBourrin(BT::CompositeNode* node)
{
    BT::Status status = BT::Running;
    while (status == BT::Running)
    {
        status = node->tick();
        ::_getch();
    }
}

int main()
{
    Enemy toto;

    BT::Sequence behavior;

    auto* killAnEnemy = new BT::Retry(&behavior);
    auto* victoryDance = new BT::VictoryDance(&behavior);

    new BT::ReloadGunIfEmpty(killAnEnemy);
    new BT::Fire(killAnEnemy, &toto);
    new BT::IsEnemyDead(killAnEnemy, &toto);

    testBourrin(&behavior);
}


