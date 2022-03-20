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

#include <SparkyStudios/AI/Behave/Core/SSBehaviorTreeFactory.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
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
} // namespace SparkyStudios::AI::Behave::BehaviorTree
