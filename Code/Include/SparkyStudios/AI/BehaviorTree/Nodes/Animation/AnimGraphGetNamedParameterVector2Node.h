#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterVector2Node : public AnimGraphGetNamedParameterNode<AZ::Vector2>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterVector2Node, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterVector2Node, "{b1502554-025f-447d-96e9-abffbca21cde}", AnimGraphGetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterVector2";

        AnimGraphGetNamedParameterVector2Node(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
