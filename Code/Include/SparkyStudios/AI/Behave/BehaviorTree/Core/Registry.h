#pragma once

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/BlackboardProperty.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Core
{
    class Factory;

    /**
     * @brief The function used to build properties.
     */
    using BlackboardPropertyBuilder = AZStd::function<AZStd::unique_ptr<Blackboard::BlackboardProperty>(const char*)>;

    /**
     * @brief The function used to build behavior tree nodes.
     */
    using NodeBuilder = AZStd::function<AZStd::unique_ptr<Node>(const std::string&, const BehaviorTreeNodeConfiguration&)>;

    /**
     * @brief Register the blackboard properties and nodes used in behavior tree files.
     */
    class Registry final
    {
        friend class Factory;

    public:
        AZ_CLASS_ALLOCATOR(Registry, AZ::SystemAllocator, 0);
        AZ_RTTI(Registry, "{1A24E981-17FC-4D02-AC71-FF0E575B09BA}");

        virtual ~Registry() = default;

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         *
         * @param type The property value type.
         * @param uuid The property type UUID.
         * @param builder The property builder.
         */
        void RegisterProperty(const AZStd::string& type, const AZ::Uuid& uuid, const BlackboardPropertyBuilder& builder);

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         * The type parameter should have the AZ_RTTI macro for this override to work.
         *
         * @tparam T The type of the property value. Must have the AZ_RTTI macro.
         * @param type The property value type.
         * @param builder The property builder.
         */
        template<typename T>
        void RegisterProperty(const AZStd::string& type, const BlackboardPropertyBuilder& builder)
        {
            static_assert(AZStd::is_base_of_v<Blackboard::BlackboardProperty, T>, "T must be derived from BlackboardProperty");
            static_assert(!AZStd::is_abstract_v<T>, "T must not be abstract");

            RegisterProperty(type, azrtti_typeid<T>(), builder);
        }

        /**
         * @brief Registers a new blackboard property in this registry, given its type.
         * The type parameter should have the AZ_RTTI macro for this override to work.
         * This override will use the default builder.
         *
         * @tparam T The type of the property value. Must have the AZ_RTTI macro.
         * @param type The property value type.
         */
        template<typename T>
        void RegisterProperty(const AZStd::string& type)
        {
            static_assert(AZStd::is_base_of_v<Blackboard::BlackboardProperty, T>, "T must be derived from BlackboardProperty");
            static_assert(!AZStd::is_abstract_v<T>, "T must not be abstract");

            const BlackboardPropertyBuilder builder = [](const char* name)
            {
                return AZStd::make_unique<T>(name);
            };

            RegisterProperty(type, azrtti_typeid<T>(), builder);
        }

        /**
         * @brief Get the property builder for the given type.
         *
         * @param type The type to get the builder for.
         * @return const SSBehaviorTreeBlackboardPropertyBuilder&
         * @throws std::runtime_error if the type is not registered.
         */
        [[nodiscard]] const BlackboardPropertyBuilder& GetPropertyBuilder(const AZStd::string& type) const;

        /**
         * @brief Get the Uuid for the given type.
         *
         * @param type The type to get the UUID for.
         *
         * @return const AZ::Uuid&
         */
        [[nodiscard]] const AZ::Uuid& GetPropertyUuid(const AZStd::string& type) const;

        /**
         * @brief Add a node for the registration process.
         *
         * @tparam T The type of the node.
         * @param name The name of the node in the behavior tree.
         */
        template<typename T>
        void DelayNodeRegistration(const AZStd::string& name)
        {
            static_assert(AZStd::is_base_of_v<Node, T>, "T must be derived from Node");
            static_assert(!AZStd::is_abstract_v<T>, "T must not be abstract");
            static_assert(
                AZStd::is_same_v<decltype(T::Reflect), void(AZ::ReflectContext*)>,
                "T must implement the 'static void Reflect(AZ::ReflectContext*)' method.");

            NodeBuilder builder = [](const std::string& nodeName, const BehaviorTreeNodeConfiguration& config)
            {
                return AZStd::make_unique<T>(nodeName, config);
            };

            BT::TreeNodeManifest manifest{ BT::getType<T>(), name.c_str(), BT::getProvidedPorts<T>() };

            _delayedRegisterers.insert(AZStd::make_pair(name, AZStd::make_pair(manifest, builder)));
            _delayedReflectors.insert(AZStd::make_pair(name, T::Reflect));

            _registeredNodeUuid.insert(AZStd::make_pair(name, azrtti_typeid<T>()));
        }

        /**
         * @brief Get the Uuid of the specified node.
         *
         * @param type The type of the node to get the Uuid.
         *
         * @return const AZ::Uuid&
         */
        [[nodiscard]] const AZ::Uuid& GetNodeUuid(const AZStd::string& type) const;

        /**
         * @brief Perform the node registration process.
         *
         * @param nodes The list of nodes to register. These nodes must have
         * been added for registration using DelayNodeRegistration().
         */
        void EnableNodes(const AZStd::vector<AZStd::string>& nodes);

        /**
         * @brief Enable all the nodes of the registry.
         */
        void EnableNodes();

        /**
         * @brief Get the list of registered properties.
         *
         * @return AZStd::vector<AZStd::string>
         */
        [[nodiscard]] AZStd::vector<AZStd::string> GetRegisteredProperties() const;

        /**
         * @brief Get the list of registered nodes.
         *
         * @return AZStd::vector<AZStd::string>
         */
        [[nodiscard]] AZStd::vector<AZStd::string> GetRegisteredNodes() const;

        /**
         * @brief Returns the internal BT factory for advanced usage.
         *
         * @return const AZStd::unique_ptr<BT::BehaviorTreeFactory>&
         */
        [[nodiscard]] const AZStd::unique_ptr<BT::BehaviorTreeFactory>& GetNativeFactory() const;

    private:
        /**
         * @brief Register a new node in this registry. The node should be saved for a delayed registration
         * before this call.
         *
         * @param name The name of the node in the behavior tree file.
         * @param context The reflection context.
         */
        void RegisterNode(const AZStd::string& name, AZ::ReflectContext* context = nullptr);

        AZStd::unordered_map<AZStd::string, BlackboardPropertyBuilder> _registeredTypeBuilders;
        AZStd::unordered_map<AZStd::string, AZ::Uuid> _registeredTypeUuid;

        AZStd::unordered_map<AZStd::string, NodeBuilder> _registeredNodeBuilders;
        AZStd::unordered_map<AZStd::string, AZ::Uuid> _registeredNodeUuid;

        AZStd::unordered_map<AZStd::string, AZStd::pair<BT::TreeNodeManifest, NodeBuilder>> _delayedRegisterers;
        AZStd::unordered_map<AZStd::string, AZ::ReflectionFunction> _delayedReflectors;

        AZStd::unique_ptr<BT::BehaviorTreeFactory> _factory = AZStd::make_unique<BT::BehaviorTreeFactory>();

        AZ_INLINE static const AZ::Uuid InvalidUuid = AZ::Uuid::CreateNull();
        AZ_INLINE static const BlackboardPropertyBuilder InvalidBuilder;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Core
