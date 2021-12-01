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

#include <StdAfx.h>

#include <Assets/SSBehaviorTreeAssetHandler.h>
#include <SSBehaviorTreeEditorComponent.h>

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h>
#include <SparkyStudios/AI/BehaviorTree/SSBehaviorTreeBus.h>

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/XML/rapidxml.h>
#include <AzCore/std/sort.h>

#include <AzFramework/StringFunc/StringFunc.h>

#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/API/ViewPaneOptions.h>
#include <AzToolsFramework/AssetBrowser/Entries/SourceAssetBrowserEntry.h>

namespace SparkyStudios::AI::BehaviorTree
{
    using namespace SparkyStudios::AI::BehaviorTree::Blackboard;
    using namespace SparkyStudios::AI::BehaviorTree::Core;

    //=========================================================================
    // ~SSBehaviorTreeEditorComponent
    //=========================================================================
    SSBehaviorTreeEditorComponent::~SSBehaviorTreeEditorComponent()
    {
        ClearDataElements();
    }

    //=========================================================================
    // AZ::Component Implementation
    //=========================================================================

    void SSBehaviorTreeEditorComponent::Init()
    {
        // Ensure m_behaviorTreeComponent's asset reference is up to date on deserialize.
        // SetBehaviorTree(m_behaviorTreeAsset);
    }

    void SSBehaviorTreeEditorComponent::Activate()
    {
        m_behaviorTreeComponent.Init();

        if (m_behaviorTreeAsset.GetId().IsValid())
        {
            // Re-retrieve the asset in case it was reloaded while we were inactive.
            m_behaviorTreeAsset = AZ::Data::AssetManager::Instance().GetAsset<Assets::SSBehaviorTreeAsset>(
                m_behaviorTreeAsset.GetId(), AZ::Data::AssetLoadBehavior::Default);
            SetBehaviorTree(m_behaviorTreeAsset);

            AZ::Data::AssetBus::Handler::BusConnect(m_behaviorTreeAsset.GetId());
            m_behaviorTreeAsset.QueueLoad();
        }
        else
        {
            SetBehaviorTree();
        }
    }

    void SSBehaviorTreeEditorComponent::Deactivate()
    {
        AZ::Data::AssetBus::Handler::BusDisconnect();

        ClearDataElements();
    }

    //=========================================================================
    // AzToolsFramework::Components::EditorComponentBase
    //=========================================================================

    void SSBehaviorTreeEditorComponent::BuildGameEntity(AZ::Entity* entity)
    {
        AZ::SerializeContext* context = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(context, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

        if (!context)
        {
            AZ_Error("SSBehaviorTree", false, "Unable to get a serialize context from component application.");
            return;
        }

        entity->AddComponent(context->CloneObject(&m_behaviorTreeComponent));
    }

    void SSBehaviorTreeEditorComponent::SetPrimaryAsset(const AZ::Data::AssetId& assetId)
    {
        AZ::Data::Asset<Assets::SSBehaviorTreeAsset> asset =
            AZ::Data::AssetManager::Instance().GetAsset<Assets::SSBehaviorTreeAsset>(assetId, AZ::Data::AssetLoadBehavior::Default);

        if (asset)
        {
            SetBehaviorTree(asset);
            BehaviorTreeHasChanged();
            EBUS_EVENT(AzToolsFramework::ToolsApplicationRequests::Bus, AddDirtyEntity, GetEntityId());
        }
    }

    //=========================================================================
    // AZ::Data::AssetEvents
    //=========================================================================

    void SSBehaviorTreeEditorComponent::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        SetBehaviorTree(asset);
        LoadBehaviorTree();
    }

    void SSBehaviorTreeEditorComponent::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        SetBehaviorTree(asset);
        LoadBehaviorTree();
    }

    void SSBehaviorTreeEditorComponent::OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        // only notify for asset errors for the asset we care about.
#if defined(AZ_ENABLE_TRACING)
        if ((asset.GetId().IsValid()) && (asset == m_behaviorTreeAsset))
        {
            AZ_Error(
                "SSBehaviorTree", false, "Failed to load asset for SSBehaviorTreeComponent: %s",
                m_behaviorTreeAsset.ToString<AZStd::string>().c_str());
        }
#else // else if AZ_ENABLE_TRACING is not currently defined...
        AZ_UNUSED(asset);
#endif
    }

    //=========================================================================
    // SSBehaviorTreeEditorComponent
    //=========================================================================

    const char* SSBehaviorTreeEditorComponent::CacheString(const char* str)
    {
        if (str == nullptr)
        {
            return nullptr;
        }

        return m_cachedStrings.insert(AZStd::make_pair(str, AZStd::string(str))).first->second.c_str();
    }

    bool SSBehaviorTreeEditorComponent::LoadAttribute(
        const char* name, AZ::Edit::ElementData& ed, SSBehaviorTreeBlackboardProperty* prop, const char* type)
    {
        AZ_UNUSED(type);

        // just attribute key/value pairs
        if (prop->IsNumber())
        {
            SSBehaviorTreeBlackboardPropertyNumber* p = azrtti_cast<SSBehaviorTreeBlackboardPropertyNumber*>(prop);
            ed.m_attributes.push_back(AZ::Edit::AttributePair(AZ::Crc32(name), aznew AZ::Edit::AttributeData<double>(p->m_value)));
        }
        else if (prop->IsBoolean())
        {
            SSBehaviorTreeBlackboardPropertyBoolean* p = azrtti_cast<SSBehaviorTreeBlackboardPropertyBoolean*>(prop);
            ed.m_attributes.push_back(AZ::Edit::AttributePair(AZ::Crc32(name), aznew AZ::Edit::AttributeData<bool>(p->m_value)));
        }
        else if (prop->IsString())
        {
            AZStd::string value;
            SSBehaviorTreeBlackboardPropertyString* p = azrtti_cast<SSBehaviorTreeBlackboardPropertyString*>(prop);
            value = CacheString(p->m_value.c_str());
            ed.m_attributes.push_back(AZ::Edit::AttributePair(AZ::Crc32(name), aznew AZ::Edit::AttributeData<const char*>(value.c_str())));
        }
        else
        {
            return false;
        }

        return true;
    }

    const AZ::Edit::ElementData* SSBehaviorTreeEditorComponent::GetDataElement(const void* element, const AZ::Uuid& typeUuid) const
    {
        auto it = m_dataElements.find(element);
        if (it != m_dataElements.end())
        {
            if (it->second.m_uuid == typeUuid)
            {
                return &it->second.m_editData;
            }
        }
        return nullptr;
    }

    void SSBehaviorTreeEditorComponent::ClearDataElements()
    {
        for (auto it = m_dataElements.begin(); it != m_dataElements.end(); ++it)
        {
            if (it->second.m_isAttributeOwner)
            {
                it->second.m_editData.ClearAttributes();
            }
        }

        m_dataElements.clear();

        // The display tree might still be holding onto pointers to our attributes that we just cleared above, so force a refresh to remove
        // them. However, only force the refresh if we have a valid entity.  If we don't have an entity, this component isn't currently
        // being shown or edited, so a refresh is at best superfluous, and at worst could cause a feedback loop of infinite refreshes.
        if (GetEntity())
        {
            EBUS_EVENT(AzToolsFramework::ToolsApplicationEvents::Bus, InvalidatePropertyDisplay, AzToolsFramework::Refresh_EntireTree);
        }
    }

    void SSBehaviorTreeEditorComponent::LoadBehaviorTree()
    {
        LoadProperties();
        EBUS_EVENT(AzToolsFramework::ToolsApplicationEvents::Bus, InvalidatePropertyDisplay, AzToolsFramework::Refresh_EntireTree);
    }

    void SSBehaviorTreeEditorComponent::LoadProperties()
    {
        ClearDataElements();

        const Assets::SSBehaviorTreeAsset* asset = m_behaviorTreeAsset.GetAs<Assets::SSBehaviorTreeAsset>();
        const AZStd::vector<char>& buffer = asset->GetBuffer();

        if (!buffer.empty())
        {
            // Loads properties from XML file and populate the blackboard
            AZ::rapidxml::xml_document<char> doc;
            AZStd::string xml = AZStd::string(buffer.cbegin(), buffer.cend());
            doc.parse<0>(xml.data());

            if (AZ::rapidxml::xml_node<char>* rootNode = doc.first_node("root"))
            {
                // AZ::rapidxml::xml_node<char>* treeModelNode = rootNode->first_node("TreeNodeModel");
                AZ::rapidxml::xml_node<char>* blackboardNode = rootNode->first_node("Blackboard");

                if (blackboardNode)
                {
                    SSBehaviorTreeFactory factory;
                    EBUS_EVENT_RESULT(factory, SSBehaviorTreeRequestBus, GetFactory);

                    if (AZ::rapidxml::xml_attribute<char>* blackboardName = blackboardNode->first_attribute("name"))
                    {
                        m_behaviorTreeComponent.m_btBlackboard.m_name = blackboardName->value();
                    }

                    AZ::rapidxml::xml_node<char>* propertyNode;
                    while ((propertyNode = blackboardNode->first_node("Property")))
                    {
                        if (AZ::rapidxml::xml_attribute<char>* name = propertyNode->first_attribute("name"))
                        {
                            AZ::rapidxml::xml_attribute<char>* type = propertyNode->first_attribute("type");
                            const char* valueType = "AZStd::string";

                            if (!type)
                            {
                                AZ_Warning(
                                    "SSBehaviorTree", false,
                                    "SS BehaviorTree Parse Error: Found a Property node (%s) without the \"type\" attribute. Assuming the "
                                    "type to be an AZStd::string.",
                                    name->value());
                            }
                            else
                            {
                                valueType = type->value();
                            }

                            const AZ::Uuid& valueTypeUuId = factory.GetRegistry()->GetPropertyUuid(valueType);
                            AZ_Error(
                                "SSBehaviorTree", !valueTypeUuId.IsNull(),
                                "The type \"%s\" is used in the behavior tree but was not registered. You will need to register the type "
                                "with SSBehaviorTreeRegistry::RegisterProperty<T>(\"%s\") to avoid serialization errors.",
                                valueType, valueType);

                            bool needToCreateProperty;
                            SSBehaviorTreeBlackboardProperty* btProp = m_behaviorTreeComponent.m_btBlackboard.GetProperty(name->value());

                            if (!btProp)
                            {
                                needToCreateProperty = true;
                            }
                            else
                            {
                                if (azrtti_typeid(btProp) == valueTypeUuId)
                                {
                                    needToCreateProperty = false;
                                }
                                else
                                {
                                    if (auto findIt = AZStd::find(
                                            m_behaviorTreeComponent.m_btBlackboard.m_properties.begin(),
                                            m_behaviorTreeComponent.m_btBlackboard.m_properties.end(), btProp);
                                        findIt != m_behaviorTreeComponent.m_btBlackboard.m_properties.end())
                                    {
                                        delete *findIt;
                                        m_behaviorTreeComponent.m_btBlackboard.m_properties.erase(findIt);
                                    }

                                    needToCreateProperty = true;
                                }
                            }

                            if (needToCreateProperty)
                            {
                                if (AZStd::unique_ptr<SSBehaviorTreeBlackboardProperty> prop =
                                        factory.CreateProperty(valueType, name->value()))
                                {
                                    if (AZ::rapidxml::xml_attribute<char>* v = propertyNode->first_attribute("default"))
                                    {
                                        prop->SetValueFromString(v->value());
                                    }

                                    m_behaviorTreeComponent.m_btBlackboard.m_properties.emplace_back(AZStd::move(prop.release()));
                                }
                            }

                            if (SSBehaviorTreeBlackboardProperty* prop = m_behaviorTreeComponent.m_btBlackboard.GetProperty(name->value()))
                            {
                                prop->m_visibility = AZ::Edit::PropertyVisibility::ShowChildrenOnly;

                                if (AZ::rapidxml::xml_attribute<char>* s = propertyNode->first_attribute("suffix"))
                                {
                                    prop->m_suffix = s->value();
                                }

                                if (AZ::rapidxml::xml_attribute<char>* d = propertyNode->first_attribute("description"))
                                {
                                    prop->m_description = d->value();
                                }

                                if (AZ::rapidxml::xml_attribute<char>* p = propertyNode->first_attribute("private"))
                                {
                                    prop->m_visibility = BT::convertFromString<bool>(p->value())
                                        ? AZ::Edit::PropertyVisibility::Hide
                                        : AZ::Edit::PropertyVisibility::ShowChildrenOnly;
                                }

                                ElementInfo ei;
                                ei.m_editData.m_name = CacheString(name->value());
                                ei.m_editData.m_description = prop->m_description.c_str();
                                ei.m_editData.m_elementId = AZ::Edit::UIHandlers::Default;
                                ei.m_sortOrder = FLT_MAX;

                                // add the attributes to the map of default values
                                ei.m_uuid = prop->GetDataTypeUuid();
                                ei.m_isAttributeOwner = true;
                                m_dataElements.insert(AZStd::make_pair(prop->GetDataAddress(), ei));

                                // Also register to the script property itself, so friendly data can be displayed at its own level.
                                ei.m_uuid = azrtti_typeid(prop);
                                ei.m_isAttributeOwner = false;
                                m_dataElements.insert(AZStd::make_pair(prop, ei));

                                LoadAttribute(name->value(), ei.m_editData, prop, valueType);
                            }

                            blackboardNode->remove_node(propertyNode);
                        }
                        else
                        {
                            AZ_Error(
                                "SSBehaviorTree", false,
                                "SS BehaviorTree Parse Error: Found a Property node without the \"name\" attribute. This may lead to "
                                "errors during the behavior tree execution.");
                        }
                    }
                }
            }

            // Remove old properties to make the property grid always up to date
            RemovedOldProperties(m_behaviorTreeComponent.m_btBlackboard);

            // Sort the list of property in alphabetical order
            SortProperties(m_behaviorTreeComponent.m_btBlackboard);

            // Parses the list of provided properties in the blackboard
            ParseProperties(m_behaviorTreeComponent.m_btBlackboard);
        }
    }

    void SSBehaviorTreeEditorComponent::ParseProperties([[maybe_unused]] const SSBehaviorTreeBlackboard& blackboard)
    {
    }

    void SSBehaviorTreeEditorComponent::RemovedOldProperties(SSBehaviorTreeBlackboard& blackboard)
    {
        for (auto it = blackboard.m_properties.begin(); it != blackboard.m_properties.end();)
        {
            SSBehaviorTreeBlackboardProperty* prop = *it;
            if (m_dataElements.find(prop->GetDataAddress()) == m_dataElements.end())
            {
                delete prop;
                it = blackboard.m_properties.erase(it); // we did not find a data element it must an old property
            }
            else
            {
                ++it;
            }
        }
    }

    void SSBehaviorTreeEditorComponent::SortProperties(SSBehaviorTreeBlackboard& blackboard)
    {
        // sort properties
        AZStd::sort(
            blackboard.m_properties.begin(), blackboard.m_properties.end(),
            [&](const SSBehaviorTreeBlackboardProperty* lhs, const SSBehaviorTreeBlackboardProperty* rhs) -> bool
            {
                auto lhsElementInfoPair = m_dataElements.find(lhs);
                auto rhsElementInfoPair = m_dataElements.find(rhs);

                AZ_Assert(
                    lhsElementInfoPair != m_dataElements.end() && rhsElementInfoPair != m_dataElements.end(),
                    "We have blackboard properties that have do not have dataElements, this should not be possible!");

                if (lhsElementInfoPair->second.m_sortOrder == FLT_MAX && rhsElementInfoPair->second.m_sortOrder == FLT_MAX)
                {
                    // use alphabetical sort
                    return lhs->m_name < rhs->m_name;
                }
                else
                {
                    // sort based on the order defined by user
                    return lhsElementInfoPair->second.m_sortOrder < rhsElementInfoPair->second.m_sortOrder;
                }
            });
    }

    void SSBehaviorTreeEditorComponent::SetBehaviorTree(const AZ::Data::Asset<Assets::SSBehaviorTreeAsset>& tree)
    {
        m_behaviorTreeAsset = tree;
        m_behaviorTreeComponent.SetBehaviorTree(tree);

        m_customName = "SS BehaviorTree";

        if (tree)
        {
            bool outcome = false;
            AZStd::string folderFoundIn;
            AZ::Data::AssetInfo assetInfo;

            AzToolsFramework::AssetSystemRequestBus::BroadcastResult(
                outcome, &AzToolsFramework::AssetSystemRequestBus::Events::GetSourceInfoBySourceUUID, m_behaviorTreeAsset.GetId().m_guid,
                assetInfo, folderFoundIn);

            if (outcome)
            {
                AZStd::string name;
                AzFramework::StringFunc::Path::GetFileName(assetInfo.m_relativePath.c_str(), name);
                m_customName += AZStd::string::format(" - %s", name.c_str());
            }
        }
    }

    AZ::u32 SSBehaviorTreeEditorComponent::BehaviorTreeHasChanged()
    {
        AZ::Data::AssetBus::Handler::BusDisconnect();

        // Only clear properties and data elements if the asset we're changing to is not the same one we already had set on our behavior
        // tree component The only time we shouldn't do this is when someone has set the same script on the component through the editor
        if (m_behaviorTreeAsset != m_behaviorTreeComponent.GetBehaviorTree())
        {
            m_behaviorTreeComponent.m_btBlackboard.Clear();
            ClearDataElements();
        }

        if (m_behaviorTreeAsset.GetId().IsValid())
        {
            SetBehaviorTree(m_behaviorTreeAsset);

            AZ::Data::AssetBus::Handler::BusConnect(m_behaviorTreeAsset.GetId());
            m_behaviorTreeAsset.QueueLoad();
        }
        else
        {
            SetBehaviorTree();
        }

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }

    const AZ::Edit::ElementData* SSBehaviorTreeEditorComponent::GetScriptPropertyEditData(
        const void* handlerPtr, const void* elementPtr, const AZ::Uuid& elementType)
    {
        const auto* owner = reinterpret_cast<const SSBehaviorTreeEditorComponent*>(handlerPtr);
        return owner->GetDataElement(elementPtr, elementType);
    }

    void SSBehaviorTreeEditorComponent::Reflect(AZ::ReflectContext* reflection)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(reflection))
        {
            SSBehaviorTreeComponent::Reflect(reflection);

            sc->Class<SSBehaviorTreeEditorComponent, EditorComponentBase>()
                ->Field("component", &SSBehaviorTreeEditorComponent::m_behaviorTreeComponent)
                ->Field("asset", &SSBehaviorTreeEditorComponent::m_behaviorTreeAsset);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<SSBehaviorTreeEditorComponent>(
                      "BehaviorTree", "An implementation of behavior trees for O3DE. Using BehaviorTree.CPP.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Sparky Studios")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeEditorComponent::m_customName)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, Assets::SSBehaviorTreeAssetHandler::GetAssetTypeStatic())
                    ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/SSBehaviorTree.svg")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Editor/Icons/Viewport/SSBehaviorTree.png")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &SSBehaviorTreeEditorComponent::m_behaviorTreeAsset, "Behavior Tree",
                        "The behavior tree to run.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &SSBehaviorTreeEditorComponent::BehaviorTreeHasChanged)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &SSBehaviorTreeEditorComponent::m_behaviorTreeComponent,
                        "SS BehaviorTree properties", "The behavior tree")
                    ->SetDynamicEditDataProvider(&SSBehaviorTreeEditorComponent::GetScriptPropertyEditData)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);

                ec->Class<SSBehaviorTreeComponent>(
                      "SS BehaviorTree Component", "Add custom behavior trees to the entity, using the powerfull BehaviorTree.CPP library.")
                    ->DataElement(0, &SSBehaviorTreeComponent::m_btBlackboard, "Blackboard", "BehaviorTree properties.")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(0, &SSBehaviorTreeComponent::m_behaviorTreeAsset, "Asset", "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Hide)
                    ->Attribute(
                        AZ::Edit::Attributes::SliceFlags,
                        AZ::Edit::SliceFlags::NotPushable) // Only the editor-component's script asset needs to be slice-pushable.
                    ;

                ec->Class<SSBehaviorTreeBlackboard>("SS BehaviorTree Blackboard", "The blackboard of a behavior tree.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "SSBehaviorTreeBlackboard's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboard::m_name)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(0, &SSBehaviorTreeBlackboard::m_properties, "m_properties", "Properties in this blackboard.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);

                ec->Class<SSBehaviorTreeBlackboardProperty>("SS BehaviorTree Blackboard Property", "Base class for blackboard properties")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "SSBehaviorTreeBlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility);

                ec->Class<SSBehaviorTreeBlackboardPropertyBoolean>("SS BehaviorTree Property (bool)", "A blackboard boolean property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "SSBehaviorTreeBlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility)
                    ->DataElement(0, &SSBehaviorTreeBlackboardPropertyBoolean::m_value, "m_value", "A boolean")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::m_name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::m_suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::m_description);

                ec->Class<SSBehaviorTreeBlackboardPropertyNumber>(
                      "SS BehaviorTree Blackboard Property (number)", "A blackboard number property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "SSBehaviorTreeBlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility)
                    ->DataElement(0, &SSBehaviorTreeBlackboardPropertyNumber::m_value, "m_value", "A number")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::m_name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::m_suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::m_description);

                ec->Class<SSBehaviorTreeBlackboardPropertyString>(
                      "SS BehaviorTree Blackboard Property (string)", "A blackboard string property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "SSBehaviorTreeBlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility)
                    ->DataElement(0, &SSBehaviorTreeBlackboardPropertyString::m_value, "m_value", "A string")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::m_name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::m_suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::m_description);

                ec->Class<SSBehaviorTreeBlackboardPropertyEntityRef>(
                      "SS BehaviorTree Blackboard Property (EntityRef)", "A blackboard entity reference property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "SSBehaviorTreeBlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility)
                    ->DataElement(0, &SSBehaviorTreeBlackboardPropertyEntityRef::m_value, "m_entity", "An entity reference")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::m_name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::m_suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::m_description);

                ec->Class<SSBehaviorTreeBlackboardPropertyVector2>(
                      "SS BehaviorTree Blackboard Property (Vector2)", "A blackboard Vector2 property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "SSBehaviorTreeBlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &SSBehaviorTreeBlackboardProperty::m_visibility)
                    ->DataElement(0, &SSBehaviorTreeBlackboardPropertyVector2::m_value, "m_entity", "An two dimensional vector")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &SSBehaviorTreeBlackboardProperty::m_name)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &SSBehaviorTreeBlackboardProperty::m_suffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &SSBehaviorTreeBlackboardProperty::m_description);
            }
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree
