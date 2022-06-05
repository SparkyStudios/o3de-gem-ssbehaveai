#include <Recast.h>

#include <AzCore/Math/Transform.h>

#include <Navigation/Utils/Constants.h>
#include <Navigation/Utils/RecastMath.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    RecastVector3::RecastVector3(const AZ::Vector3& in)
    {
        mXYZ[0] = in.GetX();
        mXYZ[1] = in.GetZ();
        mXYZ[2] = in.GetY();
    }

    RecastVector3::RecastVector3(const float* in)
    {
        mXYZ[0] = in[0];
        mXYZ[1] = in[1];
        mXYZ[2] = in[2];
    }

    RecastVector3::RecastVector3(float x, float y, float z)
    {
        mXYZ[0] = x;
        mXYZ[1] = y;
        mXYZ[2] = z;
    }

    float* RecastVector3::data()
    {
        return &mXYZ[0];
    }

    const float* RecastVector3::data() const
    {
        return &mXYZ[0];
    }

    AZ::Vector3 RecastVector3::AsVector3() const
    {
        return { mXYZ[0], mXYZ[2], mXYZ[1] };
    }

    void RecastNavigationMeshGeometry::Clear()
    {
        mChunkedGeometry.reset();
        mChunkedGeometry = nullptr;

        mVertices.clear();
        mIndices.clear();
    }

    bool RecastNavigationMeshGeometry::IsEmpty() const
    {
        return mVertices.empty();
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

    RecastOffMeshConnections::RecastOffMeshConnections(const OffMeshConnections& connections)
    {
        mPoints.reserve(connections.mConnections.size());
        mRadii.reserve(connections.mConnections.size());
        mDirections.reserve(connections.mConnections.size());
        mAreas.reserve(connections.mConnections.size());
        mFlags.reserve(connections.mConnections.size());
        mIds.reserve(connections.mConnections.size());

        AZ::u32 id = 0;

        for (const auto& connection : connections.mConnections)
        {
            mPoints.push_back(RecastVector3(connection.mStart));
            mPoints.push_back(RecastVector3(connection.mEnd));

            mRadii.push_back(connection.mRadius);

            mDirections.push_back(static_cast<AZ::u8>(connection.mDirection));

            mAreas.push_back(kDefaultNavigationMeshAreaId); // TODO: Add support for area IDs
            mFlags.push_back(eNAF_JUMP); // TODO: Add support for flags

            mIds.push_back(1000 + id++);
        }
    }

    void RecastOffMeshConnections::Clear()
    {
        mPoints.clear();
        mRadii.clear();
        mDirections.clear();
        mAreas.clear();
        mFlags.clear();
        mIds.clear();
    }
} // namespace SparkyStudios::AI::Behave::Navigation
