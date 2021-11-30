#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetMotionNode.h>

#include <AzCore/Asset/AssetCommon.h>

#include <Integration/SimpleMotionComponentBus.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
{
    SimpleMotionGetMotionNode::SimpleMotionGetMotionNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void SimpleMotionGetMotionNode::Reflect(AZ::ReflectContext* reflection)
    {
        AZ_UNUSED(reflection);
    }

    void SimpleMotionGetMotionNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        registry->DelayNodeRegistration<SimpleMotionGetMotionNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList SimpleMotionGetMotionNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::OutputPort<AZ::Data::AssetId>(NODE_PORT_VALUE_NAME, NODE_PORT_VALUE_DESCRIPTION),
        }));

        return ports;
    }

    Core::SSBehaviorTreeNodeStatus SimpleMotionGetMotionNode::Tick()
    {
        AZ::Data::AssetId value;
        EBUS_EVENT_ID_RESULT(value, GetEntityId(), EMotionFX::Integration::SimpleMotionComponentRequestBus, GetMotion);
        SetOutputValue<AZ::Data::AssetId>(NODE_PORT_VALUE_NAME, value);

        return Core::SSBehaviorTreeNodeStatus::SUCCESS;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Animation
