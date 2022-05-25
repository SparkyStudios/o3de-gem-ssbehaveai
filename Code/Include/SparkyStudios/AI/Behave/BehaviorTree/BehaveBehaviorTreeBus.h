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

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Factory.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    class BehaveBehaviorTreeRequests
    {
    public:
        AZ_RTTI(SSBehaviorTreeRequests, "{CF1733ED-EEB3-4330-AF52-B0F290232A14}");
        virtual ~BehaveBehaviorTreeRequests() = default;

        /**
         * @brief Get the nodes/properties factory.
         *
         * @return const Factory&
         */
        [[nodiscard]] virtual const Core::Factory& GetFactory() const = 0;
    };

    class BehaveBehaviorTreeBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using BehaveBehaviorTreeRequestBus = AZ::EBus<BehaveBehaviorTreeRequests, BehaveBehaviorTreeBusTraits>;
    using BehaveBehaviorTreeInterface = AZ::Interface<BehaveBehaviorTreeRequests>;
} // namespace SparkyStudios::AI::Behave::BehaviorTree
