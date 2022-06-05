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

#include <Recast.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <Navigation/Assets/NavigationAreasAsset.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void NavigationAreasAsset::Reflect(AZ::ReflectContext* rc)
    {
        NavigationArea::Reflect(rc);

        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<NavigationAreasAsset, AssetData>()
                ->Version(0)
                ->Attribute(AZ::Edit::Attributes::EnableForAssetEditor, true)
                ->Field("Areas", &NavigationAreasAsset::mAreas);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<NavigationAreasAsset>("Behave AI - Navigation Areas Asset", "An asset to setup the list of navigation areas.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &NavigationAreasAsset::mAreas, "Areas", "Navigation areas.")
                    ->Attribute(AZ::Edit::Attributes::MaxLength, RC_WALKABLE_AREA);
            }
        }
    }
} // namespace SparkyStudios::AI::Behave::Navigation
