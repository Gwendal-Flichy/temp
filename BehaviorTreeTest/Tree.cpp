#include "Tree.h"

namespace BT
{
    IComponentNode::IComponentNode(CompositeNode* parent)
        : m_parent(nullptr)
    {
        setParent(parent);
    }

    IComponentNode::~IComponentNode()
    {
        setParent(nullptr);
    }

    void IComponentNode::setParent(CompositeNode* newParent)
    {
        if (m_parent)
            m_parent->remove(this);

        m_parent = newParent;

        if (m_parent)
            m_parent->add(this);
    }
}
