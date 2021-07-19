#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterVector3Node : public AnimGraphSetNamedParameterNode<AZ::Vector3>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterVector3Node, AZ::SystemAllocator, 0);
        AZ_RTTI(
            AnimGraphSetNamedParameterVector3Node, "{9b871b83-e0a0-458e-8807-2b807d7ea660}", AnimGraphSetNamedParameterNode<AZ::Vector3>);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterVector3";

        AnimGraphSetNamedParameterVector3Node(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
