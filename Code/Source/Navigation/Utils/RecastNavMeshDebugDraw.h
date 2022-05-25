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

#include <DebugDraw.h>

#include <AzFramework/Entity/EntityDebugDisplayBus.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    class RecastNavMeshDebugDraw final : public duDebugDraw
    {
    public:
        ~RecastNavMeshDebugDraw() override = default;

        void depthMask(bool state) override;

        void texture([[maybe_unused]] bool state) override
        {
        }

        void begin(duDebugDrawPrimitives prim, float size = 1.0f) override;

        void vertex(const float* pos, unsigned int color) override
        {
            AddVertex(pos[0], pos[1], pos[2], color);
        }

        void vertex(const float x, const float y, const float z, unsigned int color) override
        {
            AddVertex(x, y, z, color);
        }

        void vertex(const float* pos, unsigned int color, [[maybe_unused]] const float* uv) override
        {
            AddVertex(pos[0], pos[1], pos[2], color);
        }

        void vertex(
            const float x, const float y, const float z, unsigned int color, [[maybe_unused]] const float u, [[maybe_unused]] const float v)
            override
        {
            AddVertex(x, y, z, color);
        }

        void end() override;

        void SetDebugDisplayRequestsHandler(AzFramework::DebugDisplayRequests* debugDisplay);
        void SetEnableDepthTest(bool depthTest);

    protected:
        void AddVertex(float x, float y, float z, unsigned int color);

        bool m_depthTest = false;
        duDebugDrawPrimitives m_currentPrim = DU_DRAW_QUADS;
        float m_currentSize = 1.0f;
        AZStd::vector<AZStd::pair<AZ::Vector3, AZ::u32>> m_verticesToDraw;
        AzFramework::DebugDisplayRequests* m_debugDisplay = nullptr;
    };
} // namespace SparkyStudios::AI::Behave::Navigation
