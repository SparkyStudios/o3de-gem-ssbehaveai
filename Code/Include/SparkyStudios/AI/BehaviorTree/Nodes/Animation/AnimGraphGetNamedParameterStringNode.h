#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterStringNode : public AnimGraphGetNamedParameterNode<AZStd::string>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterStringNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterStringNode, "{b72eff30-3c72-465d-8ad4-c59ddd9f9e27}", AnimGraphGetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterString";

        AnimGraphGetNamedParameterStringNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
