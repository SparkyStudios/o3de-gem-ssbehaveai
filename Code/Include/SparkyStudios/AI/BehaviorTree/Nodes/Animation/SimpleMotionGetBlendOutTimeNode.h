#pragma once

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class SimpleMotionGetBlendOutTimeNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(SimpleMotionGetBlendOutTimeNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SimpleMotionGetBlendOutTimeNode, "{89d5a1e3-8c16-4e0a-988d-2d3ab839bb8c}", Core::SSBehaviorTreeNode);

        static constexpr const char* NODE_NAME = "SimpleMotionGetBlendOutTime";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The blackboard entry in which the value should be set.";

        SimpleMotionGetBlendOutTimeNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* reflection);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        Core::SSBehaviorTreeNodeStatus Tick();
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
