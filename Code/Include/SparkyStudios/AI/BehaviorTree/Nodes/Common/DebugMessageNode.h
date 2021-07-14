#pragma once

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h>

namespace SparkyStudios::AI::BehaviorTree::Nodes::Common
{
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

        static void RegisterNode(const AZStd::unique_ptr<Core::SSBehaviorTreeRegistry>& registry);

        enum class DebugMessageLevel
        {
            LEVEL_ERROR = 0,
            LEVEL_WARNING,
            LEVEL_INFO,
            LEVEL_SILENT,
        };

        DebugMessageNode(const std::string& name, const Core::SSBehaviorTreeNodeConfiguration& config);

        static Core::SSBehaviorTreePortsList providedPorts();

    protected:
        Core::SSBehaviorTreeNodeStatus Tick() override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Nodes::Common

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(
        SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel, "{57840eec-7a59-4b6e-9c24-66f947280ca8}");
} // namespace AZ

namespace BT
{
    template<>
    inline std::string toStr<SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel>(
        SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel value)
    {
        switch (value)
        {
        case SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_ERROR:
            return "error";
        case SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_WARNING:
            return "warning";
        case SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO:
            return "info";
        case SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_SILENT:
            return "silent";
        default:
            return "info";
        }
    }

    template<>
    inline SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel convertFromString(StringView str)
    {
        if (str.compare("error") == 0)
        {
            return SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_ERROR;
        }
        else if (str.compare("warning") == 0)
        {
            return SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_WARNING;
        }
        else if (str.compare("info") == 0)
        {
            return SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO;
        }
        else if (str.compare("silent") == 0)
        {
            return SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_SILENT;
        }

        return SparkyStudios::AI::BehaviorTree::Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO;
    }
} // namespace BT

namespace SparkyStudios::AI::BehaviorTree::Blackboard
{
    class SSBehaviorTreeBlackboardPropertyDebugMessageLevel : public SSBehaviorTreeBlackboardProperty
    {
    public:
        AZ_CLASS_ALLOCATOR(SSBehaviorTreeBlackboardPropertyDebugMessageLevel, AZ::SystemAllocator, 0);
        AZ_RTTI(
            SSBehaviorTreeBlackboardPropertyDebugMessageLevel, "{09ef7645-63e9-40ac-88f2-2e489caf101d}", SSBehaviorTreeBlackboardProperty);

        static void Reflect(AZ::ReflectContext* context);

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel();

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel(const char* name);

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel(
            const char* name, const Nodes::Common::DebugMessageNode::DebugMessageLevel& value);

        const void* GetDataAddress() const override;

        const AZ::Uuid& GetDataTypeUuid() const override;

        SSBehaviorTreeBlackboardPropertyDebugMessageLevel* Clone(const char* name = nullptr) const override;

        void AddBlackboardEntry(const BT::Blackboard::Ptr& blackboard) const override;

        void SetValueFromString(const char* value);

        Nodes::Common::DebugMessageNode::DebugMessageLevel value = Nodes::Common::DebugMessageNode::DebugMessageLevel::LEVEL_INFO;

    protected:
        void CloneDataFrom(const SSBehaviorTreeBlackboardProperty* property) override;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Blackboard
