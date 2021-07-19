#pragma once

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class SimpleMotionSetMirrorMotionNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(SimpleMotionSetMirrorMotionNode, AZ::SystemAllocator, 0);
        AZ_RTTI(SimpleMotionSetMirrorMotionNode, "{d1f12337-cd86-40f3-bd28-5dd51482aa85}", Core::SSBehaviorTreeNode);

        static constexpr const char* NODE_NAME = "SimpleMotionSetMirrorMotion";

        static constexpr const char* NODE_PORT_VALUE_NAME = "value";
        static constexpr const char* NODE_PORT_VALUE_DESCRIPTION = "The mirror state to define to the simple motion.";

        SimpleMotionSetMirrorMotionNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* reflection);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        Core::SSBehaviorTreeNodeStatus Tick();
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
