#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterBoolNode : public AnimGraphSetNamedParameterNode<bool>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterBoolNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterBoolNode, "{c051a77f-487f-4977-8c3c-81f9ec45f55d}", AnimGraphSetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterBool";

        AnimGraphSetNamedParameterBoolNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
