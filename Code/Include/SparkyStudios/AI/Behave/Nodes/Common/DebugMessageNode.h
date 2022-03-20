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

#pragma once

#include <SparkyStudios/AI/Behave/Blackboard/SSBehaviorTreeBlackboard.h>
#include <SparkyStudios/AI/Behave/Blackboard/SSBehaviorTreeBlackboardProperty.h>
#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common
{
#pragma region DebugMessageNode

    class DebugMessageNode : public Core::SSBehaviorTreeNode
    {
    public:
        AZ_CLASS_ALLOCATOR(DebugMessageNode, AZ::SystemAllocator, 0);
        AZ_RTTI(DebugMessageNode, "{a0059957-67f9-43cb-ae02-1576835b0c73}", Core::SSBehaviorTreeNode);

        static constexpr const char* NODE_NAME = "DebugMessage";

        static constexpr const char* NODE_PORT_MESSAGE_NAME = "message";
        static constexpr const char* NODE_PORT_MESSAGE_DESCRIPTION = "The message to display in the debug console.";

        static constexpr const char* NODE_PORT_LEVEL_NAME = "level";
        static constexpr const char* NODE_PORT_LEVEL_DESCRIPTION =
            "The level of the debug message. Allowed values are \"info\", \"warning\", and \"error\".";

        static void Reflect(AZ::ReflectContext* context);

        static void RegisterNode(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry);

        enum class DebugMessageLevel
        {
            LEVEL_ERROR = 0,
            LEVEL_WARNING,
            LEVEL_INFO,
            LEVEL_SILENT,
        };

        DebugMessageNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static Core::SSBehaviorTreePortsList providedPorts();

        const std::string NodeCategory() const override
        {
            return "Common";
        }

    protected:
        Core::SSBehaviorTreeNodeStatus Tick() override;
    };

#pragma endregion

#pragma region SSBehaviorTreeBlackboardPropertyDebugMessageLevel

    class SSBehaviorTreeBlackboardPropertyDebugMessageLevel : public Blackboard::SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyDebugMessageLevel, AZ::SystemAllocator, 0);
        AZ_RTTI(
            SSBehaviorTreeBlackboardPropertyDebugMessageLevel,
            "{09ef7645-63e9-40ac-88f2-2e489caf101d}",
            Blackboard::SSBehaviorTreeBlackboardProperty);

        using DebugMessageLevel = DebugMessageNode::DebugMessageLevel;

        static void Reflect(AZ::ReflectContext* context);

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel();

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel(const char* name);

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel(const char* name, const DebugMessageLevel& value);

        const void* GetDataAddress() const override;

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const Blackboard::SSBehaviorTreeBlackboard& blackboard) const override;

        void SetValueFromString(const char* value);

        DebugMessageLevel m_value = DebugMessageLevel::LEVEL_INFO;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* property) override;
    };

#pragma endregion
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(
        SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel, "{57840eec-7a59-4b6e-9c24-66f947280ca8}");
} // namespace AZ

namespace BT
{
    template<>
    inline std::string toStr<SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel>(
        SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel value)
    {
        switch (value)
        {
        case SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_ERROR:
            return "error";
        case SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_WARNING:
            return "warning";
        case SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO:
            return "info";
        case SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_SILENT:
            return "silent";
        default:
            return "info";
        }
    }

    template<>
    inline SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel convertFromString(StringView str)
    {
        if (str.compare("error") == 0)
        {
            return SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_ERROR;
        }
        else if (str.compare("warning") == 0)
        {
            return SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_WARNING;
        }
        else if (str.compare("info") == 0)
        {
            return SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO;
        }
        else if (str.compare("silent") == 0)
        {
            return SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_SILENT;
        }

        return SparkyStudios::AI::Behave::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO;
    }
} // namespace BT
