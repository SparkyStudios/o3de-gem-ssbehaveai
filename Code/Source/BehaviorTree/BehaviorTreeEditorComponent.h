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

#include <BehaviorTree/BehaviorTreeComponent.h>

#include <AzCore/Serialization/DynamicSerializableField.h>
#include <AzCore/Serialization/EditContext.h>

#include <AzFramework/Entity/EntityDebugDisplayBus.h>

#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#include <AzToolsFramework/ToolsComponents/EditorVisibilityBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    using namespace Blackboard;

    class BehaviorTreeEditorComponentRequests : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(BehaviorTreeEditorComponentRequests, "{BBCB1021-D379-4AEC-A343-96CAED019859}");
        ~BehaviorTreeEditorComponentRequests() override = default;

        /**
         * @brief Get the id of the asset held by this component.
         *
         * @return const AZ::Data::AssetId&
         */
        virtual const AZ::Data::AssetId& GetAssetId() const = 0;
    };

    using BehaviorTreeEditorComponentRequestBus = AZ::EBus<BehaviorTreeEditorComponentRequests>;
    using BehaviorTreeEditorComponentInterface = AZ::Interface<BehaviorTreeEditorComponentRequests>;

    class BehaviorTreeEditorComponent
        : public AzToolsFramework::Components::EditorComponentBase
        , private AZ::Data::AssetBus::Handler
        , private BehaviorTreeEditorComponentRequestBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(BehaviorTreeEditorComponent, "{109004F0-14F0-4B5A-9A3E-533D404B4FCA}");

        // Perform reflection for this component. The rc parameter is the reflection context.
        static void Reflect(AZ::ReflectContext* rc);

        ~BehaviorTreeEditorComponent() override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;
        void SetPrimaryAsset(const AZ::Data::AssetId& /*assetId*/) override;

        // AZ::Data::AssetEvents
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;

        // AZ::Data::AssetEvents
        [[nodiscard]] const AZ::Data::AssetId& GetAssetId() const override;
        [[nodiscard]] const AZ::Data::Asset<Assets::BehaviorTreeAsset>& GetBehaviorTree() const;

        void SetBehaviorTree(const AZ::Data::Asset<Assets::BehaviorTreeAsset>& asset = {});

        static const AZ::Edit::ElementData* GetScriptPropertyEditData(
            const void* handlerPtr, const void* elementPtr, const AZ::Uuid& elementType);

    private:
        struct ElementInfo
        {
            // Type uuid for the class field that should use this edit data.
            AZ::Uuid mUuid;

            // Edit metadata (name, description, attributes, etc).
            AZ::Edit::ElementData mEditData;

            // True if this ElementInfo owns the internal attributes. We can use a single
            // ElementInfo for more than one class field, but only one owns the Attributes.
            bool mIsAttributeOwner;

            // Sort order of the property as defined by using the "order" attribute, by default the order is FLT_MAX
            // which means alphabetical sort will be used
            float mSortOrder;
        };

        const char* CacheString(const char* str);
        bool LoadAttribute(const char* name, AZ::Edit::ElementData& ed, BlackboardProperty* prop, const char* type);
        void ClearDataElements();
        const AZ::Edit::ElementData* GetDataElement(const void* element, const AZ::Uuid& typeUuid) const;

        void ParseProperties(const BehaviorTree::Blackboard::Blackboard& blackboard);
        void RemovedOldProperties(BehaviorTree::Blackboard::Blackboard& blackboard);
        void SortProperties(BehaviorTree::Blackboard::Blackboard& blackboard);

        void LoadBehaviorTree();
        void LoadProperties();
        AZ::u32 BehaviorTreeHasChanged();

        AZStd::unordered_map<const void*, AZStd::string>
            _cachedStrings; // TODO: Make editor global as we can chase them across multiple areas

        AZStd::unordered_map<const void*, ElementInfo> _dataElements;

        BehaviorTreeComponent _behaviorTreeComponent;
        AZ::Data::Asset<Assets::BehaviorTreeAsset> _behaviorTreeAsset;
        AZStd::string _customName;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree

namespace AZ
{
    namespace Edit
    {
        class AttributeDynamicPropertyValue final : public Attribute
        {
        public:
            AZ_CLASS_ALLOCATOR(AttributeDynamicPropertyValue, AZ::SystemAllocator, 0)
            AZ_RTTI(AttributeDynamicPropertyValue, "{3C70A26C-18E1-4F30-B131-51A415C6300C}", Attribute);

            AttributeDynamicPropertyValue(const DynamicSerializableField& value)
                : mValue(value)
            {
            }

            ~AttributeDynamicPropertyValue() override
            {
                mValue.DestroyData();
            }

            template<class T>
            bool Get(T& value) const
            {
                // We deal only with exact types no base types, etc.
                // We can do that if needed, but introduces lots of complications
                // which we are not convinced they are worth it.
                // if (dhtypeid(AZStd::remove_pointer<T>::type) == mValue.m_typeId)
                if (AzTypeInfo<T>::Uuid() == mValue.m_typeId)
                {
                    GetValue(value, AZStd::is_pointer<T>::type());
                    return true;
                }

                return false;
            }

            template<class T>
            void GetValue(T& value, AZStd::false_type /*AZStd::is_pointer<T>::type()*/)
            {
                value = *static_cast<T*>(mValue.m_data);
            }

            template<class T>
            void GetValue(T& value, AZStd::true_type /*AZStd::is_pointer<T>::type()*/)
            {
                value = reinterpret_cast<T>(mValue.m_data);
            }

            DynamicSerializableField mValue;
        };
    } // namespace Edit
} // namespace AZ
