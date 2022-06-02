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

#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/containers/vector.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    enum class OffMeshConnectionDirection : AZ::u8
    {
        Unidirectional = 0,
        Bidirectional = 1,
    };

    class OffMeshConnection
    {
    public:
        AZ_TYPE_INFO(OffMeshConnection, "{CE91DBC6-F815-47E0-AD36-5ABA986E981A}");

        static void Reflect(AZ::ReflectContext* rc);

        AZ::Vector3 mStart;

        float mRadius;

        AZ::Vector3 mEnd;

        OffMeshConnectionDirection mDirection;
    };

    class OffMeshConnections
    {
    public:
        AZ_TYPE_INFO(OffMeshConnections, "{9305F058-7CBA-477C-89F1-FA6BBA9563EF}");

        static void Reflect(AZ::ReflectContext* rc);

        AZStd::vector<OffMeshConnection> mConnections;
    };
} // namespace SparkyStudios::AI::Behave::Navigation

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(SparkyStudios::AI::Behave::Navigation::OffMeshConnectionDirection, "{144B504A-1B11-478E-AF3E-4F708684AFB1}");
} // namespace AZ
