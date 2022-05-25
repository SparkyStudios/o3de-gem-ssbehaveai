#pragma once

#include <behaviortree_cpp_v3/bt_factory.h>

#include <AzCore/RTTI/RTTI.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/Blackboard.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    /**
     * @brief Create blackboard properties and nodes for a behavior tree.
     *
     * The possible properties and nodes to create are defined by the
     * registry instance given in the constructor.
     */
    class Factory final
    {
    public:
        AZ_CLASS_ALLOCATOR(Factory, AZ::SystemAllocator, 0);
        AZ_RTTI(Factory, "{D5BC9F90-B601-4FB8-9D06-30DBEBD1FC43}");

        /**
         * @brief Construct a new Factory.
         *
         * @param registry The registry from which get the possible properties and nodes to create.
         * If no value or nullptr are given, then a new empty registry is created.
         */
        Factory(Registry* registry = nullptr);

        /**
         * @brief Destroy the Factory.
         * This will also destroy the registry.
         */
        virtual ~Factory();

        /**
         * @brief Get the wrapped registry instance.
         *
         * @return const AZStd::shared_ptr<Registry>&
         */
        [[nodiscard]] const AZStd::shared_ptr<Registry>& GetRegistry() const;

        /**
         * @brief Creates a new blackboard property from the given type.
         *
         * @param type The property value type, as specified when registering with RegisterProperty.
         * @param name The instance name of the property.
         *
         * @return AZStd::unique_ptr<Blackboard::BlackboardProperty>
         */
        AZStd::unique_ptr<BehaviorTree::Blackboard::BlackboardProperty> CreateProperty(const AZStd::string& type, const char* name) const;

        /**
         * @brief Create a new behavior tree node from the given name. This method will only work for a node
         * that was registered with RegisterNode.
         *
         * @param name The node name.
         * @param instanceName The instance name of the node.
         * @param config The configuration for the node.
         *
         * @return AZStd::unique_ptr<Node>
         */
        [[nodiscard]] AZStd::unique_ptr<Node> CreateNode(
            const AZStd::string& name, const AZStd::string& instanceName, const BehaviorTreeNodeConfiguration& config = {}) const;

        /**
         * @brief Create a behavior tree from the given XML text.
         *
         * @param text The XML text to parse into a behavior tree.
         * @param blackboard The blackboard instance which this behavior tree will use.
         *
         * @return BT::Tree
         */
        [[nodiscard]] BT::Tree CreateTreeFromText(const AZStd::string& text, const BehaviorTree::Blackboard::Blackboard& blackboard = {}) const;

    private:
        AZStd::shared_ptr<Registry> _registry;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core
