#include <Recast.h>

#include <AzCore/Math/Transform.h>

#include <Navigation/Utils/RecastMath.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    RecastVector3::RecastVector3(const AZ::Vector3& in)
    {
        mX = in.GetX();
        mY = in.GetZ();
        mZ = in.GetY();
    }

    RecastVector3::RecastVector3(const float* in)
    {
        mX = in[0];
        mY = in[1];
        mZ = in[2];
    }

    RecastVector3::RecastVector3(float x, float y, float z)
    {
        mX = x;
        mY = y;
        mZ = z;
    }

    float* RecastVector3::data()
    {
        return &mX;
    }

    const float* RecastVector3::data() const
    {
        return &mX;
    }

    AZ::Vector3 RecastVector3::AsVector3() const
    {
        return { mX, mZ, mY };
    }

    void RecastGeometry::Clear()
    {
        mVertices.clear();
        mIndices.clear();
    }

    RecastAreaConvexVolume::RecastAreaConvexVolume()
        : mHMin(0.0f)
        , mHMax(0.0f)
        , mArea(RC_WALKABLE_AREA)
    {
    }

    RecastAreaConvexVolume::RecastAreaConvexVolume(const AZ::PolygonPrism& prism, const AZ::Transform& transform)
        : mArea(RC_WALKABLE_AREA)
    {
        mHMin = transform.GetTranslation().GetZ();
        mHMax = mHMin + prism.GetHeight();
        mVertices.reserve(prism.m_vertexContainer.Size());

        for (size_t i = 0, l = prism.m_vertexContainer.Size(); i < l; i++)
        {
            const AZ::Vector2& vertex = prism.m_vertexContainer[i];
            AZ::Vector3 point = transform.TransformPoint(AZ::Vector3(vertex.GetX(), vertex.GetY(), 0.0f));
            mVertices.push_back(RecastVector3(point));
        }
    }
} // namespace SparkyStudios::AI::Behave::Navigation
