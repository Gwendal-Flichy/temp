#pragma once

#include "Tree.h"
#include "NPC.h"

namespace BT
{
    class IfGunEmpty : public BehaviorNodeDecorator<NPC, IConditionalNode>
    {
    public:
        IfGunEmpty(ICompositeNode* node)
            : BehaviorNodeDecorator<NPC, IConditionalNode>(node)
        {
        }

        bool condition()
        {
            return getGameObject()->isClipEmpty();
        }
    };

    class ReloadGun : public BehaviorNodeDecorator<NPC, IActionNode>
    {
    public:
        ReloadGun(ICompositeNode* parent) : BehaviorNodeDecorator<NPC, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            getGameObject()->reloadGun();
            m_delay--;
            if (m_delay >= 1)
            {
                std::cout << "Gun reloading .";
                return Running;
            }

            std::cout << ". DONE!" << std::endl;
            m_delay = 2;
            return Success;
        }

    private:
        int m_delay = 2;
    };

    class Fire : public BehaviorNodeDecorator<NPC, IActionNode>
    {
    public:
        Fire(ICompositeNode* parent) : BehaviorNodeDecorator<NPC, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            if (!getGameObject()->isCurrentTargetValid())
                return Failed;

            if (getGameObject()->fire())
                return Success;

            return Failed;
        }
    };

    class IsEnemyDead : public BehaviorNodeDecorator<NPC, IActionNode>
    {
    public:
        IsEnemyDead(ICompositeNode* parent) : BehaviorNodeDecorator<NPC, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            if (getGameObject()->getCurrentTarget()->PV <= 0)
            {
                std::cout << "Enemy killed!" << std::endl;
                return Success;
            }

            return Failed;
        }
    };

    class FindEnemy : public BehaviorNodeDecorator<NPC, IActionNode>
    {
    public:
        FindEnemy(ICompositeNode* parent) : BehaviorNodeDecorator<NPC, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            getGameObject()->findValidTarget();
            if (getGameObject()->getCurrentTarget() == nullptr)
            {
                std::cout << "No enemy left!" << std::endl;
                return Failed;
            }

            std::cout << "Enemy spotted!" << std::endl;
            return Success;
        }
    };

    class VictoryDance : public BehaviorNodeDecorator<NPC, IActionNode>
    {
    public:
        VictoryDance(ICompositeNode* parent) : BehaviorNodeDecorator<NPC, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            if (m_delay <= 0)
            {
                std::cout << std::endl;
                return Success;
            }
            std::vector<std::string> danceSteps = { "\\o/", "|o/", "\\o|", "-o-" };
            std::cout << danceSteps[m_delay % 4] << " ";
            m_delay--;

            return Running;
        }

    private:
        int m_delay = 12;
    };
}
