#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterVector3Node : public AnimGraphGetNamedParameterNode<AZ::Vector3>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterVector3Node, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterVector3Node, "{836230a3-51be-4d74-bdd8-d91b4b208e4c}", AnimGraphGetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterVector3";

        AnimGraphGetNamedParameterVector3Node(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
