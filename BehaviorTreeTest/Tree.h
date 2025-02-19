#pragma once

#include <iostream>
#include <vector>
#include <type_traits>

struct Enemy
{
    int PV = 3;
};

namespace BT
{

    class CompositeNode;

    enum Status
    {
        Success
      , Failed
      , Running
    };

    struct IComponentNode
    {
        IComponentNode(CompositeNode* parent = nullptr);

        void setParent(CompositeNode* newParent);

        virtual ~IComponentNode();

        virtual void display() = 0;

        virtual Status tick() = 0;

    protected: 
        CompositeNode* m_parent;
    };

    struct ActionNode : IComponentNode
    {
        ActionNode(CompositeNode* node = nullptr)
            : IComponentNode(node)
        {
        }

        void display() override
        {
            std::cout << "child" << std::endl;
        }
    };

    class CompositeNode : public IComponentNode
    {
    public:
        friend IComponentNode;

        CompositeNode(CompositeNode* node = nullptr)
            : IComponentNode(node)
        {
        }

        ~CompositeNode()
        {
            for(std::make_signed_t<size_t> idx = m_children.size() - 1 ; idx >= 0 ; --idx)
            {
                delete m_children[idx];
                std::cout << "je detruis les trucs qui trainent" << std::endl;
            }
        }

        void display() override
        {
            for (auto& child : m_children)
                child->display();
        }

    protected:
        const std::vector<IComponentNode*>& getChildren() const
        {
            return m_children;
        }

    private:
        void add(IComponentNode* node)
        {
            m_children.push_back(node);
        }

        void remove(IComponentNode* node)
        {
            auto it = std::find(m_children.begin(), m_children.end(), node);
            if (it == m_children.end())
                throw;

            m_children.erase(it);
        }

    private:
        std::vector<IComponentNode*> m_children;

    };
    
    class ControlNode;

    class IStrategy
    {
    public:
        virtual ~IStrategy() = default;
        virtual Status execute(ControlNode* node) = 0;
    };

    class ControlNode : public CompositeNode
    {
    public:
        ControlNode(CompositeNode* parent, IStrategy* failedStrategy, IStrategy* runningStrategy, IStrategy* successStrategy)
            : CompositeNode(parent)
            , m_nextNodeToTick(0)
            , m_failedStrategy(failedStrategy)
            , m_runningStrategy(runningStrategy)
            , m_successStrategy(successStrategy)
        {
        }

        ~ControlNode()
        {
            delete m_failedStrategy;
            delete m_runningStrategy;
            delete m_successStrategy;
        }

        void reset()
        {
            m_nextNodeToTick = 0;
        }

        void incrementChildrenIndex()
        {
            m_nextNodeToTick++;
        }

        bool isLastChildren() const
        {
            return m_nextNodeToTick == getChildren().size() - 1;
        }

        virtual Status tick()
        {
            Status childStatus = getChildren()[m_nextNodeToTick]->tick();

            if (childStatus == Failed)
                return m_failedStrategy->execute(this);

            if (childStatus == Running)
                return m_runningStrategy->execute(this);

            if (childStatus == Success)
                return m_successStrategy->execute(this);

            throw;
        }

    private:
        size_t m_nextNodeToTick;
        IStrategy* m_failedStrategy;
        IStrategy* m_runningStrategy;
        IStrategy* m_successStrategy;
    };

    struct FailedStrategy : public IStrategy
    {
        virtual Status execute(ControlNode* node)
        {
            node->reset();
            return Failed;
        }
    };

    struct RetryStrategy : public IStrategy
    {
        virtual Status execute(ControlNode* node)
        {
            node->reset();
            return Running;
        }
    };

    struct RunningStrategy : public IStrategy
    {
        virtual Status execute(ControlNode* node)
        {
            return Running;
        }
    };

    struct SuccessStrategy : public IStrategy
    {
        virtual Status execute(ControlNode* node)
        {
            if (node->isLastChildren())
            {
                node->reset();
                return Success;
            }
            
            node->incrementChildrenIndex();
            return Running;
        }
    };

    /*
    class Sequence : public CompositeNode
    {
    public:
        Sequence(CompositeNode* parent = nullptr) : CompositeNode(parent), m_nextNodeToTick(0)
        {
        }

        virtual Status tick()
        {
            Status childStatus = getChildren()[m_nextNodeToTick]->tick();

            if (childStatus == Failed)
            {
                m_nextNodeToTick = 0;
                return Failed;
            }

            if (childStatus == Running)
                return Running;

            m_nextNodeToTick++;
            if (m_nextNodeToTick >= getChildren().size())
            {
                m_nextNodeToTick = 0;
                return Success;
            }

            return Running;

        }

    private:
        size_t m_nextNodeToTick;
    };
    */

    class Sequence : public ControlNode
    {
    public:
        Sequence(CompositeNode* parent = nullptr) :
            ControlNode(parent, new FailedStrategy, new RunningStrategy, new SuccessStrategy)
        {}
    };

    class Retry : public ControlNode
    {
    public:
        Retry(CompositeNode* parent = nullptr) :
            ControlNode(parent, new RetryStrategy, new RunningStrategy, new SuccessStrategy)
        {
        }
    };

    /*
    class Retry : public CompositeNode
    {
    public:
        Retry(CompositeNode* parent = nullptr) : CompositeNode(parent), m_nextNodeToTick(0)
        {
        }

        virtual Status tick()
        {
            Status childStatus = getChildren()[m_nextNodeToTick]->tick();

            if (childStatus == Failed)
            {
                m_nextNodeToTick = 0;
                return Running;
            }

            if (childStatus == Running)
                return Running;

            m_nextNodeToTick++;
            if (m_nextNodeToTick >= getChildren().size())
            {
                m_nextNodeToTick = 0;
                return Success;
            }

            return Running;
        }

    private:
        size_t m_nextNodeToTick;
    };
    */
    class ReloadGunIfEmpty : public ActionNode
    {
    public:
        ReloadGunIfEmpty(CompositeNode* parent) : ActionNode(parent)
        {
        }

        Status tick() override
        {
            m_delay--;
            if (m_delay >= 1)
                return Running;
            
            std::cout << "Gun reloaded!" << std::endl;
            return Success;
        }

    private:
        int m_delay = 2;
    };

    class Fire : public ActionNode
    {
    public:
        Fire(CompositeNode* parent, Enemy* enemy) : ActionNode(parent), m_enemy(enemy)
        {
        }

        Status tick() override
        {
            std::cout << "Fire the gun!" << std::endl;
            m_enemy->PV--;
            return Success;
        }

    private:
        Enemy* m_enemy;
    };

    class IsEnemyDead : public ActionNode
    {
    public:
        IsEnemyDead(CompositeNode* parent, Enemy* enemy) : ActionNode(parent), m_enemy(enemy)
        {
        }

        Status tick() override
        {
            if (m_enemy->PV <= 0)
            {
                std::cout << "Enemy killed!" << std::endl;
                return Success;
            }
            
            return Failed;
        }

    private:
        int m_PV = 3;
        Enemy* m_enemy;
    };

    class VictoryDance : public ActionNode
    {
    public:
        VictoryDance(CompositeNode* parent) : ActionNode(parent)
        {
        }

        Status tick() override
        {
            if (m_delay <= 0)
            {
                std::cout << std::endl;
                return Success;
            }
            std::vector<std::string> danceSteps = { "\\o/", "|o/", "\\o|", "-o-"};
            std::cout << danceSteps[m_delay % 4] << " ";
            m_delay--;

            return Running;
        }

    private:
        int m_delay = 12;
    };
}