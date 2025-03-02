#pragma once
#pragma once

#include "Tree.h"
#include "Boss.h"




namespace BT
{


    class Walk : public BehaviorNodeDecorator<Boss, IActionNode>
    {
    public:
        Walk(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            getGameObject()->setSpeed(30);
            return Success;
        }


    };

    class Running : public BehaviorNodeDecorator<Boss, IActionNode>
    {
    public:
        Running(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            getGameObject()->setSpeed(50);
            return Success;
        }


    };
    class Idle : public BehaviorNodeDecorator<Boss, IActionNode>
    {
    public:
        Idle(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            getGameObject()->setSpeed(0);
            return Success;
        }


    };

    class IsPlayerDead : public BehaviorNodeDecorator<Boss, IActionNode>
    {
    public:
        IsPlayerDead(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            if (!getGameObject()->isCurrentTargetValid())
            {
                return Success;
            }
            return Failed;

        }
    };



    class FireShotgun : public BehaviorNodeDecorator<Boss, IActionNode>
    {
    public:
        FireShotgun(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            if (!getGameObject()->isCurrentTargetValid())
                return Failed;

            
            if (m_delay==0)
            {
	            getGameObject()->fireWithShotgun();
                m_delay = 50;
            }
            m_delay--;
            return Success;


        }
    private:
        int m_delay= 50;
    };

    class FirePistol : public BehaviorNodeDecorator<Boss, IActionNode>
    {
    public:
        FirePistol(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            if (!getGameObject()->isCurrentTargetValid())
                return Failed;

            getGameObject()->fireWithPistol();
            return Success;


        }
    };

    class FindEnemy : public BehaviorNodeDecorator<Boss, IActionNode>
    {
    public:
        FindEnemy(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
        {
        }

        Status tick() override
        {
            
            if (!getGameObject()->findValidTarget())
            {
                
                return Failed;
            }

            
            return Success;
        }
    };
	class PlayerDictance : public BehaviorNodeDecorator<Boss, IConditionalNode>
    {
    public:
        PlayerDictance(ICompositeNode* node,float nb,Operation operation)
            : BehaviorNodeDecorator<Boss, IConditionalNode>(node)
			,m_nb(nb)
			,m_operation(operation)
        {
        }

        bool condition()
        {
            float Distance = getGameObject()->getDistanceToPlayer().getLength();

            if(m_operation==sup)
            {
                return  (Distance > m_nb);

            }
            if (m_operation == inf)
            {
                return  (Distance < m_nb);

            }

            return false;
        }
	private:
        float m_nb;
        Operation m_operation;
    };
}

//    class IfGunEmpty : public BehaviorNodeDecorator<Boss, IConditionalNode>
//    {
//    public:
//        IfGunEmpty(ICompositeNode* node)
//            : BehaviorNodeDecorator<Boss, IConditionalNode>(node)
//        {
//        }
//
//        bool condition()
//        {
//            return getGameObject()->isClipEmpty();
//        }
//    };
//
//    class ReloadGun : public BehaviorNodeDecorator<Boss, IActionNode>
//    {
//    public:
//        ReloadGun(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
//        {
//        }
//
//        Status tick() override
//        {
//            getGameObject()->reloadGun();
//            m_delay--;
//            if (m_delay >= 1)
//            {
//                std::cout << "Gun reloading .";
//                return Running;
//            }
//
//            std::cout << ". DONE!" << std::endl;
//            m_delay = 2;
//            return Success;
//        }
//
//    private:
//        int m_delay = 2;
//    };
//
//    class Fire : public BehaviorNodeDecorator<Boss, IActionNode>
//    {
//    public:
//        Fire(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
//        {
//        }
//
//        Status tick() override
//        {
//            if (!getGameObject()->isCurrentTargetValid())
//                return Failed;
//
//            if (getGameObject()->fire())
//                return Success;
//
//            return Failed;
//        }
//    };
//
//    
//
//    class FindEnemy : public BehaviorNodeDecorator<Boss, IActionNode>
//    {
//    public:
//        FindEnemy(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
//        {
//        }
//
//        Status tick() override
//        {
//            getGameObject()->findValidTarget();
//            if (getGameObject()->getCurrentTarget() == nullptr)
//            {
//                std::cout << "No enemy left!" << std::endl;
//                return Failed;
//            }
//
//            std::cout << "Enemy spotted!" << std::endl;
//            return Success;
//        }
//    };
//
//    class VictoryDance : public BehaviorNodeDecorator<Boss, IActionNode>
//    {
//    public:
//        VictoryDance(ICompositeNode* parent) : BehaviorNodeDecorator<Boss, IActionNode>(parent)
//        {
//        }
//
//        Status tick() override
//        {
//            if (m_delay <= 0)
//            {
//                std::cout << std::endl;
//                return Success;
//            }
//            std::vector<std::string> danceSteps = { "\\o/", "|o/", "\\o|", "-o-" };
//            std::cout << danceSteps[m_delay % 4] << " ";
//            m_delay--;
//
//            return Running;
//        }
//
//    private:
//        int m_delay = 12;
//    };
//}
