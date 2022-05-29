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

#include <SparkyStudios/AI/Behave/Navigation/NavigationAgent.h>

#include <AzCore/EBus/EBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    /**
     * @brief EBus is used to request registered navigation mesh areas.
     */
    class NavigationAgentProviderRequests : public AZ::EBusTraits
    {
    public:
        // EBusTraits
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        //! allows multiple threads to call
        using MutexType = AZStd::recursive_mutex;

        virtual ~NavigationAgentProviderRequests() = default;

        /**
         * @brief Gets the names of all registered navigation agents.
         *
         * @param[out] names The names of the registered navigation agents.
         */
        virtual void GetRegisteredNavigationAgentNames(NavigationAgentNameSet& names) const = 0;

        /**
         * @brief Gets all the registered navigation agents.
         *
         * @param[out] agents The registered navigation agents.
         */
        virtual void GetRegisteredNavigationAgents(NavigationAgentList& agents) const = 0;

        /**
         * @brief Gets the navigation agent with the given name.
         *
         * @param name The name of the navigation agent to get. If no navigation agent with the provided name is found,
         * and invalid one will be returned.
         *
         * @return The found navigation agent, or an invalid one if not found.
         */
        [[nodiscard]] virtual NavigationAgent GetNavigationAgent(const AZStd::string& name) const = 0;
    };

    typedef AZ::EBus<NavigationAgentProviderRequests> NavigationAgentProviderRequestBus;
} // namespace SparkyStudios::AI::Behave::Navigation
