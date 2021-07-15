#pragma once

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    class AnimGraphSetNamedParameterRotationEulerNode : public AnimGraphSetNamedParameterNode<AZ::Vector3>
    {
    public:
        AZ_CLASS_ALLOCATOR(AnimGraphSetNamedParameterRotationEulerNode, AZ::SystemAllocator, 0);
        AZ_RTTI(AnimGraphSetNamedParameterRotationEulerNode, "{8b375c4a-b061-4664-b015-153189760769}", AnimGraphSetNamedParameterNode);

        static constexpr const char* NODE_NAME = "AnimGraphSetNamedParameterRotationEuler";

        AnimGraphSetNamedParameterRotationEulerNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        void SetParameter() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
