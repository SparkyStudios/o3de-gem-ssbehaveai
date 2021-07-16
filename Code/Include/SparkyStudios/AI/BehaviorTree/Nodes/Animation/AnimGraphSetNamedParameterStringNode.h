#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterStringNode : public AnimGraphSetNamedParameterNode<AZStd::string>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterStringNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterStringNode, "{397cec57-b565-4cb4-a4be-0a725c9ea6c1}", AnimGraphSetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterString";

        AnimGraphSetNamedParameterStringNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
