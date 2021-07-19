#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphGetNamedParameterRotationEulerNode : public AnimGraphGetNamedParameterNode<AZ::Vector3>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphGetNamedParameterRotationEulerNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphGetNamedParameterRotationEulerNode, "{78a08bdb-ac11-4aef-9c04-0651a6ee22b4}", AnimGraphGetNamedParameterNode<AZ::Vector3>);

        static constexpr const char* NODE_NAME = "AnimGraphGetNamedParameterRotationEuler";

        AnimGraphGetNamedParameterRotationEulerNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void GetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
