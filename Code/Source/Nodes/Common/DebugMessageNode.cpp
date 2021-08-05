#include <StdAfx.h>

#include <SparkyStudios/AI/BehaviorTree/Nodes/Common/DebugMessageNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Common
{
#pragma region DebugMessageNode

    DebugMessageNode::DebugMessageNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void DebugMessageNode::Reflect(AZ::ReflectContext* context)
    {
        SSBehaviorTreeBlackboardPropertyDebugMessageLevel::Reflect(context);
    }

    void DebugMessageNode::RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register properties
        registry->RegisterProperty<SSBehaviorTreeBlackboardPropertyDebugMessageLevel>("DebugMessageNode::DebugMessageLevel");

        // 2 - Add node for delayed registration
        registry->DelayNodeRegistration<DebugMessageNode>(NODE_NAME);
    }

    Core::SSBehaviorTreePortsList DebugMessageNode::providedPorts()
    {
        Core::SSBehaviorTreePortsList ports = Core::SSBehaviorTreeNode::providedPorts();

        ports.merge(Core::SSBehaviorTreePortsList({
            BT::InputPort<AZStd::string>(NODE_PORT_MESSAGE_NAME, NODE_PORT_MESSAGE_DESCRIPTION),
            BT::InputPort<DebugMessageLevel>(NODE_PORT_LEVEL_NAME, DebugMessageLevel::LEVEL_INFO, NODE_PORT_LEVEL_DESCRIPTION),
        }));

        return ports;
    }

    Core::SSBehaviorTreeNodeStatus DebugMessageNode::Tick()
    {
        Core::Optional<AZStd::string> message = GetInputValue<AZStd::string>(NODE_PORT_MESSAGE_NAME);
        Core::Optional<DebugMessageLevel> type = GetInputValue<DebugMessageLevel>(NODE_PORT_LEVEL_NAME);

        if (message.has_value())
        {
            switch (type.value_or(DebugMessageLevel::LEVEL_INFO))
            {
            case DebugMessageLevel::LEVEL_ERROR:
                AZ_Error("SSBehaviorTree", false, "[%s:%s]: %s", RegisteredNodeName(), NodeName(), message.value().c_str());
            case DebugMessageLevel::LEVEL_WARNING:
                AZ_Warning("SSBehaviorTree", false, "[%s:%s]: %s", RegisteredNodeName(), NodeName(), message.value().c_str());
            default:
            case DebugMessageLevel::LEVEL_INFO:
                AZ_Printf("SSBehaviorTree", "[%s:%s]: %s", RegisteredNodeName(), NodeName(), message.value().c_str());
            case DebugMessageLevel::LEVEL_SILENT:
                return Core::SSBehaviorTreeNodeStatus::SUCCESS;
            }
        }

        return Core::SSBehaviorTreeNodeStatus::SUCCESS;
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyDebugMessageLevel

    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Enum<DebugMessageLevel>()
                ->Version(1)
                ->Value("silent", DebugMessageLevel::LEVEL_SILENT)
                ->Value("info", DebugMessageLevel::LEVEL_INFO)
                ->Value("warning", DebugMessageLevel::LEVEL_WARNING)
                ->Value("error", DebugMessageLevel::LEVEL_ERROR);

            sc->Class<SSBehaviorTreeBlackboardPropertyDebugMessageLevel, SSBehaviorTreeBlackboardProperty>()->Version(1)->Field(
                "value", &SSBehaviorTreeBlackboardPropertyDebugMessageLevel::m_value);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Enum<DebugMessageLevel>("[DebugMessageNode] Debug Message Level", "The message level of the DebugMessage node.")
                    ->Value("Silent", DebugMessageLevel::LEVEL_SILENT)
                    ->Value("Information", DebugMessageLevel::LEVEL_INFO)
                    ->Value("Warning", DebugMessageLevel::LEVEL_WARNING)
                    ->Value("Error", DebugMessageLevel::LEVEL_ERROR);

                ec->Class<SSBehaviorTreeBlackboardPropertyDebugMessageLevel>(
                      "SS BehaviorTree Blackboard Property (DebugMessageLevel)", "A blackboard property.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &SSBehaviorTreeBlackboardPropertyDebugMessageLevel::m_value, "value",
                        "A debug message level.")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::m_name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::m_suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::m_description);
            }
        }
    }

    SSBehaviorTreeBlackboardPropertyDebugMessageLevel::SSBehaviorTreeBlackboardPropertyDebugMessageLevel()
    {
    }

    SSBehaviorTreeBlackboardPropertyDebugMessageLevel::SSBehaviorTreeBlackboardPropertyDebugMessageLevel(const char* name)
        : SSBehaviorTreeBlackboardProperty(name)
    {
    }

    SSBehaviorTreeBlackboardPropertyDebugMessageLevel::SSBehaviorTreeBlackboardPropertyDebugMessageLevel(
        const char* name, const DebugMessageLevel& value)
        : SSBehaviorTreeBlackboardProperty(name)
        , m_value(value)
    {
    }

    const void* SSBehaviorTreeBlackboardPropertyDebugMessageLevel::GetDataAddress() const
    {
        return &m_value;
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyDebugMessageLevel::GetDataTypeUuid() const
    {
        return azrtti_typeid<DebugMessageLevel>();
    }

    SSBehaviorTreeBlackboardPropertyDebugMessageLevel* SSBehaviorTreeBlackboardPropertyDebugMessageLevel::Clone(const char* name) const
    {
        return aznew SSBehaviorTreeBlackboardPropertyDebugMessageLevel(name ? name : m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::AddBlackboardEntry(const Blackboard::SSBehaviorTreeBlackboard& blackboard) const
    {
        blackboard.m_blackboard->set<DebugMessageLevel>(m_name.c_str(), m_value);
    }

    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::SetValueFromString(const char* value)
    {
        m_value = BT::convertFromString<DebugMessageLevel>(value);
    }

    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* property)
    {
        const auto* p = azrtti_cast<const SSBehaviorTreeBlackboardPropertyDebugMessageLevel*>(property);
        AZ_Error("SSBehaviorTree", p, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (p)
        {
            m_value = p->m_value;
        }
    }

#pragma endregion
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Common
