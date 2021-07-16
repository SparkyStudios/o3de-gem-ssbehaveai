#include <StdAfx.h>

#include <Source/Assets/SSBehaviorTreeAsset.h>

namespace SparkyStudios::AI::BehaviorTree::Assets
{
    static constexpr const char* _default_content = "<root main_tree_to_execute=\"MainTree\">\n"
                                                    "    <BehaviorTree ID=\"MainTree\">\n"
                                                    "    </BehaviorTree>\n"
                                                    "</root>";

    /**
     * @brief Construct a new SSBehaviorTreeAsset::SSBehaviorTreeAsset object
     *
     * @param assetId The unique ID of the asset.
     */
    SSBehaviorTreeAsset::SSBehaviorTreeAsset(const AZ::Data::AssetId& assetId)
        : AZ::Data::AssetData(assetId)
        , m_buffer(_default_content, _default_content + strlen(_default_content))
    {
    }
} // namespace SparkyStudios::AI::BehaviorTree::Assets
