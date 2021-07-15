#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterVector2Node : public AnimGraphSetNamedParameterNode<AZ::Vector2>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterVector2Node, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterVector2Node, "{b5ab134c-c73a-41c1-b1cf-7aaf6aa715b2}", AnimGraphSetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterVector2";

        AnimGraphSetNamedParameterVector2Node(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
