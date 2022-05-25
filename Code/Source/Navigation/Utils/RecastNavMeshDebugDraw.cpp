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

#include <Navigation/Utils/RecastNavMeshDebugDraw.h>
#include <Navigation/Utils/RecastMath.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void RecastNavMeshDebugDraw::depthMask([[maybe_unused]] bool state)
    {
        if (!m_depthTest)
            return;

        if (state)
            m_debugDisplay->DepthTestOn();
        else
            m_debugDisplay->DepthTestOff();
    }

    void RecastNavMeshDebugDraw::begin(duDebugDrawPrimitives primitives, float size)
    {
        m_currentPrim = primitives;
        m_currentSize = size;
        m_verticesToDraw.clear();
    }

    void RecastNavMeshDebugDraw::end()
    {
        if (m_debugDisplay == nullptr)
            return;

        switch (m_currentPrim)
        {
        case DU_DRAW_POINTS:
            {
                for (auto&& i : m_verticesToDraw)
                {
                    AZ::Color color = AZ::Color::CreateZero();
                    color.FromU32(i.second);

                    m_debugDisplay->SetColor(color);
                    m_debugDisplay->DrawBall(i.first, m_currentSize / 100, true);
                }
            }
            break;
        case DU_DRAW_TRIS:
            {
                for (size_t i = 2, l = m_verticesToDraw.size(); i < l; i += 3)
                {
                    AZ::Color color = AZ::Color::CreateZero();
                    color.FromU32(m_verticesToDraw[i - 2].second);

                    m_debugDisplay->SetColor(color);
                    m_debugDisplay->DrawTri(m_verticesToDraw[i - 2].first, m_verticesToDraw[i - 1].first, m_verticesToDraw[i - 0].first);
                }
            }
            break;
        case DU_DRAW_QUADS:
            {
                for (size_t i = 3, l = m_verticesToDraw.size(); i < l; i += 4)
                {
                    AZ::Color color = AZ::Color::CreateZero();
                    color.FromU32(m_verticesToDraw[i - 3].second);

                    m_debugDisplay->SetColor(color);
                    m_debugDisplay->DrawQuad(
                        m_verticesToDraw[i - 3].first, m_verticesToDraw[i - 2].first, m_verticesToDraw[i - 1].first,
                        m_verticesToDraw[i - 0].first);
                }
            }
            break;
        case DU_DRAW_LINES:
            {
                m_debugDisplay->SetLineWidth(m_currentSize);
                for (size_t i = 1, l = m_verticesToDraw.size(); i < l; i += 2)
                {
                    AZ::Color color1 = AZ::Color::CreateZero();
                    color1.FromU32(m_verticesToDraw[i - 1].second);
                    AZ::Color color2 = AZ::Color::CreateZero();
                    color2.FromU32(m_verticesToDraw[i - 0].second);

                    m_debugDisplay->DrawLine(
                        m_verticesToDraw[i - 1].first, m_verticesToDraw[i - 0].first, color1.GetAsVector4(), color2.GetAsVector4());
                }
            }
            break;
        }
    }

    void RecastNavMeshDebugDraw::SetDebugDisplayRequestsHandler(AzFramework::DebugDisplayRequests* debugDisplay)
    {
        m_debugDisplay = debugDisplay;
    }

    void RecastNavMeshDebugDraw::SetEnableDepthTest(bool depthTest)
    {
        m_depthTest = depthTest;
    }

    void RecastNavMeshDebugDraw::AddVertex(float x, float y, float z, unsigned int color)
    {
        const float temp[3] = { x, y, z };
        const RecastVector3 v(temp);
        m_verticesToDraw.push_back(AZStd::make_pair(v.AsVector3(), color));
    }
} // namespace SparkyStudios::AI::Behave::Navigation
