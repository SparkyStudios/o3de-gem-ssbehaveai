#include <SparkyStudios/AI/BehaviorTree/Nodes/Common/DebugMessageNode.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Common
{
    DebugMessageNode::DebugMessageNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config)
        : Core::SSBehaviorTreeNode(name, config)
    {
    }

    void DebugMessageNode::Reflect(AZ::ReflectContext* context)
    {
        Blackboard::SSBehaviorTreeBlackboardPropertyDebugMessageLevel::Reflect(context);
    }

    void DebugMessageNode::RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // 1 - Register properties
        registry->RegisterProperty<DebugMessageLevel>(
            "DebugMessageNode::DebugMessageLevel",
            [](const char* name)
            {
                return AZStd::make_unique<Blackboard::SSBehaviorTreeBlackboardPropertyDebugMessageLevel>(name);
            });

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
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Common

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Enum<Nodes::Common::DebugMessageNode::DebugMessageLevel>()
                ->Version(1)
                ->Value("silent", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_SILENT)
                ->Value("info", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO)
                ->Value("warning", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_WARNING)
                ->Value("error", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_ERROR);

            sc->Class<SSBehaviorTreeBlackboardPropertyDebugMessageLevel, SSBehaviorTreeBlackboardProperty>()->Version(1)->Field(
                "Value", &SSBehaviorTreeBlackboardPropertyDebugMessageLevel::value);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Enum<Nodes::Common::DebugMessageNode::DebugMessageLevel>(
                      "[DebugMessageNode] Debug Message Level", "The message level of the DebugMessage node.")
                    ->Value("Silent", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_SILENT)
                    ->Value("Information", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO)
                    ->Value("Warning", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_WARNING)
                    ->Value("Error", Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_ERROR);

                ec->Class<SSBehaviorTreeBlackboardPropertyDebugMessageLevel>(
                      "SS Behavior Tree Blackboard Property (DebugMessageLevel)", "A blackboard property.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::visibility)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &SSBehaviorTreeBlackboardPropertyDebugMessageLevel::value, "value",
                        "A debug message level.")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::description);
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
        const char* name, const Nodes::Common::DebugMessageNode::DebugMessageLevel& value)
        : SSBehaviorTreeBlackboardProperty(name)
        , value(value)
    {
    }

    const void* SSBehaviorTreeBlackboardPropertyDebugMessageLevel::GetDataAddress() const
    {
        return &value;
    }

    const AZ::Uuid& SSBehaviorTreeBlackboardPropertyDebugMessageLevel::GetDataTypeUuid() const
    {
        return azrtti_typeid<Nodes::Common::DebugMessageNode::DebugMessageLevel>();
    }

    SSBehaviorTreeBlackboardPropertyDebugMessageLevel* SSBehaviorTreeBlackboardPropertyDebugMessageLevel::Clone(
        const char* instanceName) const
    {
        return aznew SSBehaviorTreeBlackboardPropertyDebugMessageLevel(instanceName ? instanceName : name.c_str(), value);
    }

    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const
    {
        blackboard->set<Nodes::Common::DebugMessageNode::DebugMessageLevel>(name.c_str(), value);
    }

    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::SetValueFromString(const char* btValue)
    {
        value = BT::convertFromString<Nodes::Common::DebugMessageNode::DebugMessageLevel>(btValue);
    }

    void SSBehaviorTreeBlackboardPropertyDebugMessageLevel::CloneDataFrom(const SSBehaviorTreeBlackboardProperty* property)
    {
        const auto* p = azrtti_cast<const SSBehaviorTreeBlackboardPropertyDebugMessageLevel*>(property);
        AZ_Error("SSBehaviorTree", p, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (p)
        {
            value = p->value;
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard
