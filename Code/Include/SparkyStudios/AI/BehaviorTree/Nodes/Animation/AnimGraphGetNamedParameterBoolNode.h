#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterBoolNode : public AnimGraphGetNamedParameterNode<bool>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterBoolNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterBoolNode, "{a506ccd0-7380-496a-be96-dc20ea3bf049}", AnimGraphGetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterBool";

        AnimGraphGetNamedParameterBoolNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* reflection);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
