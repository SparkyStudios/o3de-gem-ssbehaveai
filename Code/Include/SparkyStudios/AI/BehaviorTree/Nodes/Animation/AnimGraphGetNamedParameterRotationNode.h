#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterRotationNode : public AnimGraphGetNamedParameterNode<AZ::Quaternion>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterRotationNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterRotationNode, "{1a04009d-28b1-45c6-bf95-52ffddb214d3}", AnimGraphGetNamedParameterNode<AZ::Quaternion>);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterRotation";

        AnimGraphGetNamedParameterRotationNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
