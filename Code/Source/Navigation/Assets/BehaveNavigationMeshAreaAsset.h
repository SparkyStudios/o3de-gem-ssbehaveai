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

#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshArea.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/RTTI/RTTI.h>

namespace AZ
{
    class ReflectContext;
}

namespace SparkyStudios::AI::Behave::Navigation
{
    class BehaveNavigationMeshAreaAsset : public AZ::Data::AssetData
    {
    public:
        AZ_RTTI(BehaveNavigationMeshAreaAsset, "{E9F8F8E0-F8E0-4D7D-A8E0-F8E0E9F8F8E0}", AZ::Data::AssetData);
        AZ_CLASS_ALLOCATOR(BehaveNavigationMeshAreaAsset, AZ::SystemAllocator, 0);

        static void Reflect(AZ::ReflectContext* rc);

        /**
         * @brief Navigation mesh areas.
         */
        AZStd::vector<BehaveNavigationMeshArea> mAreas;
    };
} // namespace SparkyStudios::AI::Behave::Navigation
