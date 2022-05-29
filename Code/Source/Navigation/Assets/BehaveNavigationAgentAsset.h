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

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/RTTI/RTTI.h>

namespace AZ
{
    class ReflectContext;
}

namespace SparkyStudios::AI::Behave::Navigation
{
    /**
     * @brief Asset containing data about a navigation agent configuration.
     */
    class BehaveNavigationAgentAsset final : public AZ::Data::AssetData
    {
    public:
        AZ_RTTI(BehaveNavigationAgentAsset, "{F47AEB2C-909C-4095-ACA8-6B896BB09CC7}", AZ::Data::AssetData);
        AZ_CLASS_ALLOCATOR(BehaveNavigationAgentAsset, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* rc);

        AZStd::vector<NavigationAgent> mAgents;
    };
} // namespace SparkyStudios::AI::Behave::Navigation
