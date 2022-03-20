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

#include <SSBehaviorTreeComponent.h>

#include <AzCore/Serialization/DynamicSerializableField.h>
#include <AzCore/Serialization/EditContext.h>

#include <AzFramework/Entity/EntityDebugDisplayBus.h>

#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#include <AzToolsFramework/ToolsComponents/EditorVisibilityBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    class SSBehaviorTreeEditorComponentRequests
    {
    public:
        AZ_RTTI(SSBehaviorTreeEditorComponentRequests, "{bbcb1021-d379-4aec-a343-96caed019859}");
        virtual ~SSBehaviorTreeEditorComponentRequests() = default;

        /**
         * @brief Get the id of the asset held by this component.
         *
         * @return const AZ::Data::AssetId&
         */
        virtual const AZ::Data::AssetId& GetAssetId() const = 0;
    };

    class SSBehaviorTreeEditorComponentBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;
        //////////////////////////////////////////////////////////////////////////
    };

    using SSBehaviorTreeEditorComponentRequestBus = AZ::EBus<SSBehaviorTreeEditorComponentRequests, SSBehaviorTreeEditorComponentBusTraits>;
    using SSBehaviorTreeEditorComponentInterface = AZ::Interface<SSBehaviorTreeEditorComponentRequests>;

    class SSBehaviorTreeEditorComponent
        : public AzToolsFramework::Components::EditorComponentBase
        , private AZ::Data::AssetBus::Handler
        , private SSBehaviorTreeEditorComponentRequestBus::Handler
    {
    public:
        AZ_EDITOR_COMPONENT(SSBehaviorTreeEditorComponent, "{109004f0-14f0-4b5a-9a3e-533d404b4fca}");

        // Perform reflection for this component. The context parameter is the reflection context.
        static void Reflect(AZ::ReflectContext* context);

        ~SSBehaviorTreeEditorComponent() override;

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AzToolsFramework::Components::EditorComponentBase
        void BuildGameEntity(AZ::Entity* gameEntity) override;
        void SetPrimaryAsset(const AZ::Data::AssetId& /*assetId*/) override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::Data::AssetEvents
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AZ::Data::AssetEvents
        const AZ::Data::AssetId& GetAssetId() const override
        {
            return m_behaviorTreeAsset.GetId();
        }
        //////////////////////////////////////////////////////////////////////////

        const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& GetBehaviorTree() const
        {
            return m_behaviorTreeComponent.GetBehaviorTree();
        }

        void SetBehaviorTree(const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& asset = {});

        static const AZ::Edit::ElementData* GetScriptPropertyEditData(
            const void* handlerPtr, const void* elementPtr, const AZ::Uuid& elementType);

    private:
        struct ElementInfo
        {
            // Type uuid for the class field that should use this edit data.
            AZ::Uuid m_uuid;
            // Edit metadata (name, description, attribs, etc).
            AZ::Edit::ElementData m_editData;
            // True if this ElementInfo owns the internal attributes. We can use a single
            // ElementInfo for more than one class field, but only one owns the Attributes.
            bool m_isAttributeOwner;
            // Sort order of the property as defined by using the "order" attribute, by default the order is FLT_MAX
            // which means alphabetical sort will be used
            float m_sortOrder;
        };

        const char* CacheString(const char* str);
        bool LoadAttribute(
            const char* name, AZ::Edit::ElementData& ed, Blackboard::SSBehaviorTreeBlackboardProperty* prop, const char* type);
        void ClearDataElements();
        const AZ::Edit::ElementData* GetDataElement(const void* element, const AZ::Uuid& typeUuid) const;

        void ParseProperties(const Blackboard::SSBehaviorTreeBlackboard& blackboard);
        void RemovedOldProperties(Blackboard::SSBehaviorTreeBlackboard& blackboard);
        void SortProperties(Blackboard::SSBehaviorTreeBlackboard& blackboard);

        void LoadBehaviorTree();
        void LoadProperties();
        AZ::u32 BehaviorTreeHasChanged();

        AZStd::unordered_map<const void*, AZStd::string>
            m_cachedStrings; // TODO: Make editor global as we can chase them across multiple areas

        AZStd::unordered_map<const void*, ElementInfo> m_dataElements;

        SSBehaviorTreeComponent m_behaviorTreeComponent;
        AZ::Data::Asset<Assets::SSBehaviorTreeAsset> m_behaviorTreeAsset;
        AZStd::string m_customName;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree

namespace AZ
{
    namespace Edit
    {
        class AttributeDynamicPropertyValue : public Attribute
        {
        public:
            AZ_CLASS_ALLOCATOR(AttributeDynamicPropertyValue, AZ::SystemAllocator, 0)
            AZ_RTTI(AttributeDynamicPropertyValue, "{3c70a26c-18e1-4f30-b131-51a415c6300c}", Attribute);

            AttributeDynamicPropertyValue(const DynamicSerializableField& value)
                : m_value(value)
            {
            }

            virtual ~AttributeDynamicPropertyValue()
            {
                m_value.DestroyData();
            }

            template<class T>
            bool Get(T& value) const
            {
                // We deal only with exact types no base types, etc.
                // We can do that if needed, but introduces lots of complications
                // which we are not convinced they are worth it.
                // if (dhtypeid(AZStd::remove_pointer<T>::type) == m_value.m_typeId)
                if (AzTypeInfo<T>::Uuid() == m_value.m_typeId)
                {
                    GetValue(value, AZStd::is_pointer<T>::type());
                    return true;
                }

                return false;
            }

            template<class T>
            void GetValue(T& value, AZStd::false_type /*AZStd::is_pointer<T>::type()*/)
            {
                value = *reinterpret_cast<T*>(m_value.m_data);
            }

            template<class T>
            void GetValue(T& value, AZStd::true_type /*AZStd::is_pointer<T>::type()*/)
            {
                value = reinterpret_cast<T>(m_value.m_data);
            }

            DynamicSerializableField m_value;
        };
    } // namespace Edit
} // namespace AZ
