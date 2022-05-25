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

#include <AzCore/Component/Component.h>
#include <SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class WalkableComponent
        : public AZ::Component
        , public BehaveWalkableRequestBus::Handler
    {
        friend class WalkableEditorComponent;

    public:
        AZ_COMPONENT(WalkableComponent, "{D993514D-ED4F-46A9-A4B6-B90FDE5267E8}");

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // BehaveWalkableRequestBus
        bool IsWalkable([[maybe_unused]] AZ::EntityId navigationMeshEntity) override
        {
            return m_isWalkable;
        }
        //////////////////////////////////////////////////////////////////////////

    private:
        bool m_isWalkable = true;
    };
} // namespace SparkyStudios::AI::Behave::Navigation
