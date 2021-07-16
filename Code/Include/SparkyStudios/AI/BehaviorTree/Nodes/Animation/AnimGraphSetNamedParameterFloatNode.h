#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterFloatNode : public AnimGraphSetNamedParameterNode<float>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterFloatNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterFloatNode, "{dae0974d-abd1-468b-8fbc-90eb82e5fd33}", AnimGraphSetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterFloat";

        AnimGraphSetNamedParameterFloatNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
