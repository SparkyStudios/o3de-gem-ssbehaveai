
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace SparkyStudios::AI::BehaviorTree
{
    class SSBehaviorTreeRequests
    {
    public:
        AZ_RTTI(SSBehaviorTreeRequests, "{cf1733ed-eeb3-4330-af52-b0f290232a14}");
        virtual ~SSBehaviorTreeRequests() = default;
        // Put your public methods here
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
