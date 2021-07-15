#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterFloatNode : public AnimGraphGetNamedParameterNode<float>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterFloatNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterFloatNode, "{b4bd8edd-6793-4943-981c-1d150d02efef}", AnimGraphGetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterFloat";

        AnimGraphGetNamedParameterFloatNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
