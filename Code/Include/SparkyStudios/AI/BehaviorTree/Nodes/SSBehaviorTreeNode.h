#pragma once

#include <behaviortree_cpp_v3/bt_factory.h>

#include <AzCore/RTTI/RTTI.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes
{
    using SSBehaviorTreeNodeConfiguration = BT::NodeConfiguration;

    class SSBehaviorTreeNode : public BT::StatefulActionNode
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SSBehaviorTreeNode, "{bdc7ef90-5955-4ee7-9118-46f0d069194f}");

        SSBehaviorTreeNode(const std::string& name, const SSBehaviorTreeNodeConfiguration& config)
            : BT::StatefulActionNode(name, config)
        {
        }

        virtual ~SSBehaviorTreeNode() = default;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes