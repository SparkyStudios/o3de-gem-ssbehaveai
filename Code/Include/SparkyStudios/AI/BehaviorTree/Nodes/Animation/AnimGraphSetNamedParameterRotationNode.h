#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterRotationNode : public AnimGraphSetNamedParameterNode<AZ::Quaternion>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterRotationNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterRotationNode, "{dc051fe8-546c-4426-9f72-2b92ab7a1d9a}", AnimGraphSetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterRotation";

        AnimGraphSetNamedParameterRotationNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
