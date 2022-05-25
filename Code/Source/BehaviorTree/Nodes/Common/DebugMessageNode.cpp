// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <StdAfx.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Common/DebugMessageNode.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common
{
#pragma region DebugMessageNode

    DebugMessageNode::DebugMessageNode(const std::string& name, const Core::BehaviorTreeNodeConfiguration& config)
        : Core::Node(name, config)
    {
    }

    void DebugMessageNode::Reflect(AZ::ReflectContext* context)
    {
        BlackboardPropertyDebugMessageLevel::Reflect(context);
    }

    void DebugMessageNode::RegisterNode(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // 1 - Register properties
        registry->RegisterProperty<BlackboardPropertyDebugMessageLevel>("DebugMessageNode::DebugMessageLevel");

        // 2 - Add node for delayed registration
        registry->DelayNodeRegistration<DebugMessageNode>(NODE_NAME);
    }

    Core::BehaviorTreePortsList DebugMessageNode::providedPorts()
    {
        Core::BehaviorTreePortsList ports = Core::Node::providedPorts();

        ports.merge(Core::BehaviorTreePortsList({
            BT::InputPort<AZStd::string>(NODE_PORT_MESSAGE_NAME, NODE_PORT_MESSAGE_DESCRIPTION),
            BT::InputPort<DebugMessageLevel>(NODE_PORT_LEVEL_NAME, DebugMessageLevel::Info, NODE_PORT_LEVEL_DESCRIPTION),
        }));

        return ports;
    }

    Core::BehaviorTreeNodeStatus DebugMessageNode::Tick()
    {
        Core::Optional<AZStd::string> message = GetInputValue<AZStd::string>(NODE_PORT_MESSAGE_NAME);
        Core::Optional<DebugMessageLevel> type = GetInputValue<DebugMessageLevel>(NODE_PORT_LEVEL_NAME);

        if (message.has_value())
        {
            switch (type.value_or(DebugMessageLevel::Info))
            {
            case DebugMessageLevel::Error:
                AZ_Error("BehaveAI [BehaviorTree]", false, "[%s:%s]: %s", RegisteredNodeName(), NodeName(), message.value().c_str());
            case DebugMessageLevel::Warning:
                AZ_Warning("BehaveAI [BehaviorTree]", false, "[%s:%s]: %s", RegisteredNodeName(), NodeName(), message.value().c_str());
            default:
            case DebugMessageLevel::Info:
                AZ_Printf("BehaveAI [BehaviorTree]", "[%s:%s]: %s", RegisteredNodeName(), NodeName(), message.value().c_str());
            case DebugMessageLevel::Silent:
                return Core::BehaviorTreeNodeStatus::SUCCESS;
            }
        }

        return Core::BehaviorTreeNodeStatus::SUCCESS;
    }

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyDebugMessageLevel

    void BlackboardPropertyDebugMessageLevel::Reflect(AZ::ReflectContext* context)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Enum<DebugMessageLevel>()
                ->Version(1)
                ->Value("silent", DebugMessageLevel::Silent)
                ->Value("info", DebugMessageLevel::Info)
                ->Value("warning", DebugMessageLevel::Warning)
                ->Value("error", DebugMessageLevel::Error);

            sc->Class<BlackboardPropertyDebugMessageLevel, BlackboardProperty>()->Version(1)->Field(
                "value", &BlackboardPropertyDebugMessageLevel::mValue);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Enum<DebugMessageLevel>("[DebugMessageNode] Debug Message Level", "The message level of the DebugMessage node.")
                    ->Value("Silent", DebugMessageLevel::Silent)
                    ->Value("Information", DebugMessageLevel::Info)
                    ->Value("Warning", DebugMessageLevel::Warning)
                    ->Value("Error", DebugMessageLevel::Error);

                ec->Class<BlackboardPropertyDebugMessageLevel>(
                      "SS BehaviorTree Blackboard Property (DebugMessageLevel)", "A blackboard property.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &BlackboardPropertyDebugMessageLevel::mValue, "Value", "A debug message level.")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BlackboardProperty::mName)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &BlackboardProperty::mSuffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &BlackboardProperty::mDescription);
            }
        }
    }

    BlackboardPropertyDebugMessageLevel::BlackboardPropertyDebugMessageLevel()
    {
    }

    BlackboardPropertyDebugMessageLevel::BlackboardPropertyDebugMessageLevel(const char* name)
        : BlackboardProperty(name)
    {
    }

    BlackboardPropertyDebugMessageLevel::BlackboardPropertyDebugMessageLevel(const char* name, const DebugMessageLevel& value)
        : BlackboardProperty(name)
        , mValue(value)
    {
    }

    const void* BlackboardPropertyDebugMessageLevel::GetDataAddress() const
    {
        return &mValue;
    }

    const AZ::Uuid& BlackboardPropertyDebugMessageLevel::GetDataTypeUuid() const
    {
        return azrtti_typeid<DebugMessageLevel>();
    }

    BlackboardPropertyDebugMessageLevel* BlackboardPropertyDebugMessageLevel::Clone(const char* name) const
    {
        return aznew BlackboardPropertyDebugMessageLevel(name ? name : mName.c_str(), mValue);
    }

    void BlackboardPropertyDebugMessageLevel::AddBlackboardEntry(const Blackboard::Blackboard& blackboard) const
    {
        blackboard.mBlackboard->set<DebugMessageLevel>(mName.c_str(), mValue);
    }

    void BlackboardPropertyDebugMessageLevel::SetValueFromString(const char* value)
    {
        mValue = BT::convertFromString<DebugMessageLevel>(value);
    }

    void BlackboardPropertyDebugMessageLevel::CloneDataFrom(const BlackboardProperty* property)
    {
        const auto* p = azrtti_cast<const BlackboardPropertyDebugMessageLevel*>(property);
        AZ_Error("BehaveAI [BehaviorTree]", p, "Invalid call to CloneData. Types must match before clone attempt is made.\n");

        if (p)
        {
            mValue = p->mValue;
        }
    }

#pragma endregion
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common
