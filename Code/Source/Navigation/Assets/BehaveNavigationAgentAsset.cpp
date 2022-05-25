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

#include <Navigation/Assets/BehaveNavigationAgentAsset.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void BehaveNavigationAgentAsset::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<BehaveNavigationAgentAsset, AssetData>()
                ->Version(0)
                ->Attribute(AZ::Edit::Attributes::EnableForAssetEditor, true)
                ->Field("Name", &BehaveNavigationAgentAsset::m_name)
                ->Field("MaxClimb", &BehaveNavigationAgentAsset::m_climb)
                ->Field("Height", &BehaveNavigationAgentAsset::m_height)
                ->Field("Radius", &BehaveNavigationAgentAsset::m_radius)
                ->Field("MaxSlopAngle", &BehaveNavigationAgentAsset::m_slopAngle);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<BehaveNavigationAgentAsset>("Behave AI - Navigation Agent Asset", "An asset to setup a navigation agent.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)

                    ->DataElement(AZ::Edit::UIHandlers::Default, &BehaveNavigationAgentAsset::m_name, "Name", "The navigation agent name.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationAgentAsset::m_height, "Height",
                        "Minimum floor to 'ceiling' height that will still allow the floor area to be considered walkable.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.1)
                    ->Attribute(AZ::Edit::Attributes::Decimals, 2)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationAgentAsset::m_climb, "Max Climb",
                        "Maximum ledge height that is considered to still be traversable.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationAgentAsset::m_radius, "Radius",
                        "The distance to erode/shrink the walkable area of the heightfield away from obstructions.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Decimals, 2)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaveNavigationAgentAsset::m_slopAngle, "Max Slop",
                        "The maximum slope that is considered walkable.")
                    ->Attribute(AZ::Edit::Attributes::Suffix, " degrees")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Max, 90);
            }
        }
    }
} // namespace SparkyStudios::AI::Behave::Navigation
