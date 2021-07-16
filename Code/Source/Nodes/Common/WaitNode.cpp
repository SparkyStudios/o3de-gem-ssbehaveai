#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Common/WaitNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Common
{
    WaitNode::WaitNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void WaitNode::Reflect(AZ::ReflectContext* context)
    {
        AZ_UNUSED(context);
    }

    void WaitNode::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Add node for delayed registration
        registry->DelayNodeRegistration<WaitNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList WaitNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::InputPort<float>(NODE_PORT_SECONDS_NAME, 0, NODE_PORT_SECONDS_DESCRIPTION),
        }));

        return ports;
    }

    void WaitNode::Start()
    {
        m_countDown = GetSeconds();
    }

    Core::SSBehaviorTreeNodeStatus WaitNode::Tick()
    {
        float seconds = GetSeconds();

        if (seconds == 0)
            return Core::SSBehaviorTreeNodeStatus::SUCCESS;

        float dt = -1;
        EBUS_EVENT_RESULT(dt, AZ::TickRequestBus, GetTickDeltaTime);

        if (dt > -1)
        {
            m_countDown -= dt;

            if (m_countDown < 0)
            {
                m_countDown = seconds;
                return Core::SSBehaviorTreeNodeStatus::SUCCESS;
            }

            return Core::SSBehaviorTreeNodeStatus::RUNNING;
        }

        return Core::SSBehaviorTreeNodeStatus::FAILURE;
    }

    float WaitNode::GetSeconds() const
    {
        Core::Optional<float> s = GetInputValue<float>(NODE_PORT_SECONDS_NAME);
        return s.has_value() ? s.value() : 0;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Common