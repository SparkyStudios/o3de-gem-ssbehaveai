#pragma once

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class SimpleMotionGetPlaySpeedNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(SimpleMotionGetPlaySpeedNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SimpleMotionGetPlaySpeedNode, "{8a0bcf03-337b-4d9a-b3a6-9b32d5d0304f}", Core::SSBehaviorTreeNode);

        static constexpr const char* NODE_NAME = "SimpleMotionGetPlaySpeed";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The blackboard entry in which the value should be set.";

        SimpleMotionGetPlaySpeedNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* reflection);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        Core::SSBehaviorTreeNodeStatus Tick();
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
