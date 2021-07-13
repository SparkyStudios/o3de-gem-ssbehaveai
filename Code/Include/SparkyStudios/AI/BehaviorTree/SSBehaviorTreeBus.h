
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeFactory.h>

namespace SparkyStudios::AI::BehaviorTree
{
    class SSBehaviorTreeRequests
    {
    public:
        AZ_RTTI(SSBehaviorTreeRequests, "{cf1733ed-eeb3-4330-af52-b0f290232a14}");
        virtual ~SSBehaviorTreeRequests() = default;

        /**
         * @brief Get the nodes/properties factory.
         *
         * @return const SSBehaviorTreeFactory&
         */
        virtual const Core::SSBehaviorTreeFactory& GetFactory() const = 0;
    };

    class SSBehaviorTreeBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using SSBehaviorTreeRequestBus = AZ::EBus<SSBehaviorTreeRequests, SSBehaviorTreeBusTraits>;
    using SSBehaviorTreeInterface = AZ::Interface<SSBehaviorTreeRequests>;
} // namespace SparkyStudios::AI::BehaviorTree
