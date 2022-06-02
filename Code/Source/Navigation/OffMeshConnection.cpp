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

#include <SparkyStudios/AI/Behave/Navigation/OffMeshConnection.h>

#include <AzCore/Serialization/EditContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void OffMeshConnection::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            // We may have been reflected already, so check first
            if (sc->FindClassData(azrtti_typeid<OffMeshConnection>()) == nullptr)
            {
                sc->Enum<OffMeshConnectionDirection>()
                    ->Version(0)
                    ->Value("Unidirectional", OffMeshConnectionDirection::Unidirectional)
                    ->Value("Bidirectional", OffMeshConnectionDirection::Bidirectional);

                sc->Class<OffMeshConnection>()
                    ->Version(0)
                    ->Field("Start", &OffMeshConnection::mStart)
                    ->Field("End", &OffMeshConnection::mEnd)
                    ->Field("Direction", &OffMeshConnection::mDirection)
                    ->Field("Radius", &OffMeshConnection::mRadius);

                if (AZ::EditContext* const ec = sc->GetEditContext())
                {
                    ec->Enum<OffMeshConnectionDirection>("Off-Mesh Connection Direction", "The direction of the connection.")
                        ->Value("Unidirectional", OffMeshConnectionDirection::Unidirectional)
                        ->Value("Bidirectional", OffMeshConnectionDirection::Bidirectional);

                    ec->Class<OffMeshConnection>("Off-Mesh Connection", "Connects two areas/tiles of a navigation mesh")
                        ->DataElement(AZ::Edit::UIHandlers::Default, &OffMeshConnection::mStart, "Start", "The connection start point.")
                        ->DataElement(AZ::Edit::UIHandlers::Default, &OffMeshConnection::mEnd, "End", "The connection end point.")
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &OffMeshConnection::mDirection, "Direction", "The connection direction.")
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &OffMeshConnection::mRadius, "Radius",
                            "The connection points radius. The radius is the allowed range in which the agent can navigate through that "
                            "connect. If the radius is lower than the agent radius, the connection will never be used.");
                }
            }
        }
    }

    void OffMeshConnections::Reflect(AZ::ReflectContext* rc)
    {
        OffMeshConnection::Reflect(rc);

        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            // We may have been reflected already, so check first
            if (sc->FindClassData(azrtti_typeid<OffMeshConnections>()) == nullptr)
            {
                sc->Class<OffMeshConnections>()->Version(0)->Field("Connections", &OffMeshConnections::mConnections);

                if (AZ::EditContext* const ec = sc->GetEditContext())
                {
                    ec->Class<OffMeshConnections>("Off-Mesh Connections", "Connects two areas/tiles of a navigation mesh")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ->DataElement(
                            AZ::Edit::UIHandlers::Default, &OffMeshConnections::mConnections, "Off-Mesh Connections",
                            "The off-mesh connections of the navigation mesh.");
                }
            }
        }
    }
} // namespace SparkyStudios::AI::Behave::Navigation
