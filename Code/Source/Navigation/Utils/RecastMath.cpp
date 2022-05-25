#include <Recast.h>

#include <Navigation/Utils/RecastMath.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void RecastGeometry::Clear()
    {
        m_vertices.clear();
        m_indices.clear();
    }

    RecastAreaConvexVolume::RecastAreaConvexVolume()
        : m_vertices()
        , m_hMin(0.0f)
        , m_hMax(0.0f)
        , m_area(RC_WALKABLE_AREA)
    {
    }

    RecastAreaConvexVolume::RecastAreaConvexVolume(const AZ::PolygonPrism& prism)
        : m_area(RC_WALKABLE_AREA)
    {
        m_hMin = 0;
        m_hMax = prism.GetHeight();
        m_vertices.reserve(prism.m_vertexContainer.Size());

        for (size_t i = 0, l = prism.m_vertexContainer.Size(); i < l; i++)
        {
            const AZ::Vector2& vertex = prism.m_vertexContainer[i];
            m_vertices.push_back(RecastVector3(vertex.GetX(), 0.0f, vertex.GetY()));
        }
    }
} // namespace SparkyStudios::AI::Behave::Navigation