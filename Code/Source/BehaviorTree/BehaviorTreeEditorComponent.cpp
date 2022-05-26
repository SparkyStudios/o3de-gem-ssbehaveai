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

#include <BehaviorTree/Assets/BehaviorTreeAssetHandler.h>
#include <BehaviorTree/BehaviorTreeEditorComponent.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/BehaveBehaviorTreeBus.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/Blackboard.h>

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/XML/rapidxml.h>
#include <AzCore/std/sort.h>

#include <AzFramework/StringFunc/StringFunc.h>

#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/AssetBrowser/Entries/SourceAssetBrowserEntry.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    using namespace Blackboard;
    using namespace Core;

    BehaviorTreeEditorComponent::~BehaviorTreeEditorComponent()
    {
        ClearDataElements();
    }

#pragma region AZ::Component Implementation

    void BehaviorTreeEditorComponent::Init()
    {
        // Ensure m_behaviorTreeComponent's asset reference is up to date on deserialize.
        // SetBehaviorTree(m_behaviorTreeAsset);
    }

    void BehaviorTreeEditorComponent::Activate()
    {
        _behaviorTreeComponent.Init();

        if (_behaviorTreeAsset.GetId().IsValid())
        {
            // Re-retrieve the asset in case it was reloaded while we were inactive.
            _behaviorTreeAsset = AZ::Data::AssetManager::Instance().GetAsset<Assets::BehaviorTreeAsset>(
                _behaviorTreeAsset.GetId(), AZ::Data::AssetLoadBehavior::Default);
            SetBehaviorTree(_behaviorTreeAsset);

            AZ::Data::AssetBus::Handler::BusConnect(_behaviorTreeAsset.GetId());
            _behaviorTreeAsset.QueueLoad();
        }
        else
        {
            SetBehaviorTree();
        }
    }

    void BehaviorTreeEditorComponent::Deactivate()
    {
        AZ::Data::AssetBus::Handler::BusDisconnect();

        ClearDataElements();
    }

#pragma endregion

#pragma region AzToolsFramework::Components::EditorComponentBase

    void BehaviorTreeEditorComponent::BuildGameEntity(AZ::Entity* entity)
    {
        AZ::SerializeContext* context = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(context, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

        if (!context)
        {
            AZ_Error("BehaveAI [BehaviorTree]", false, "Unable to get a serialize context from component application.");
            return;
        }

        entity->AddComponent(context->CloneObject(&_behaviorTreeComponent));
    }

    void BehaviorTreeEditorComponent::SetPrimaryAsset(const AZ::Data::AssetId& assetId)
    {
        if (const auto asset =
                AZ::Data::AssetManager::Instance().GetAsset<Assets::BehaviorTreeAsset>(assetId, AZ::Data::AssetLoadBehavior::Default))
        {
            SetBehaviorTree(asset);
            BehaviorTreeHasChanged();
            EBUS_EVENT(AzToolsFramework::ToolsApplicationRequests::Bus, AddDirtyEntity, GetEntityId());
        }
    }

#pragma endregion

#pragma region AZ::Data::AssetEvents

    void BehaviorTreeEditorComponent::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        SetBehaviorTree(asset);
        LoadBehaviorTree();
    }

    void BehaviorTreeEditorComponent::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        SetBehaviorTree(asset);
        LoadBehaviorTree();
    }

    void BehaviorTreeEditorComponent::OnAssetError(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        // Only notify for asset errors for the asset we care about.
#if defined(AZ_ENABLE_TRACING)
        if ((asset.GetId().IsValid()) && (asset == _behaviorTreeAsset))
        {
            AZ_Error(
                "BehaveAI [BehaviorTree]", false, "Failed to load asset for BehaviorTreeComponent: %s",
                _behaviorTreeAsset.ToString<AZStd::string>().c_str());
        }
#else // else if AZ_ENABLE_TRACING is not currently defined...
        AZ_UNUSED(asset);
#endif
    }

    const AZ::Data::AssetId& BehaviorTreeEditorComponent::GetAssetId() const
    {
        return _behaviorTreeAsset.GetId();
    }

    const AZ::Data::Asset<Assets::BehaviorTreeAsset>& BehaviorTreeEditorComponent::GetBehaviorTree() const
    {
        return _behaviorTreeComponent.GetBehaviorTree();
    }

#pragma endregion

#pragma region BehaviorTreeEditorComponent

    const char* BehaviorTreeEditorComponent::CacheString(const char* str)
    {
        if (str == nullptr)
        {
            return nullptr;
        }

        return _cachedStrings.insert(AZStd::make_pair(str, AZStd::string(str))).first->second.c_str();
    }

    bool BehaviorTreeEditorComponent::LoadAttribute(const char* name, AZ::Edit::ElementData& ed, BlackboardProperty* prop, const char* type)
    {
        AZ_UNUSED(type);

        // just attribute key/value pairs
        if (prop->IsNumber())
        {
            auto* p = azrtti_cast<BlackboardPropertyNumber*>(prop);
            ed.m_attributes.push_back(AZ::Edit::AttributePair(AZ::Crc32(name), aznew AZ::Edit::AttributeData<double>(p->mValue)));
        }
        else if (prop->IsBoolean())
        {
            auto* p = azrtti_cast<BlackboardPropertyBoolean*>(prop);
            ed.m_attributes.push_back(AZ::Edit::AttributePair(AZ::Crc32(name), aznew AZ::Edit::AttributeData<bool>(p->mValue)));
        }
        else if (prop->IsString())
        {
            AZStd::string value;
            auto* p = azrtti_cast<BlackboardPropertyString*>(prop);
            value = CacheString(p->mValue.c_str());
            ed.m_attributes.push_back(AZ::Edit::AttributePair(AZ::Crc32(name), aznew AZ::Edit::AttributeData<const char*>(value.c_str())));
        }
        else
        {
            return false;
        }

        return true;
    }

    const AZ::Edit::ElementData* BehaviorTreeEditorComponent::GetDataElement(const void* element, const AZ::Uuid& typeUuid) const
    {
        if (const auto it = _dataElements.find(element); it != _dataElements.end())
        {
            if (it->second.mUuid == typeUuid)
            {
                return &it->second.mEditData;
            }
        }

        return nullptr;
    }

    void BehaviorTreeEditorComponent::ClearDataElements()
    {
        for (auto it = _dataElements.begin(); it != _dataElements.end(); ++it)
        {
            if (it->second.mIsAttributeOwner)
            {
                it->second.mEditData.ClearAttributes();
            }
        }

        _dataElements.clear();

        // The display tree might still be holding onto pointers to our attributes that we just cleared above, so force a refresh to remove
        // them. However, only force the refresh if we have a valid entity.  If we don't have an entity, this component isn't currently
        // being shown or edited, so a refresh is at best superfluous, and at worst could cause a feedback loop of infinite refreshes.
        if (GetEntity())
        {
            EBUS_EVENT(AzToolsFramework::ToolsApplicationEvents::Bus, InvalidatePropertyDisplay, AzToolsFramework::Refresh_EntireTree);
        }
    }

    void BehaviorTreeEditorComponent::LoadBehaviorTree()
    {
        LoadProperties();
        EBUS_EVENT(AzToolsFramework::ToolsApplicationEvents::Bus, InvalidatePropertyDisplay, AzToolsFramework::Refresh_EntireTree);
    }

    void BehaviorTreeEditorComponent::LoadProperties()
    {
        ClearDataElements();

        const Assets::BehaviorTreeAsset* asset = _behaviorTreeAsset.GetAs<Assets::BehaviorTreeAsset>();

        if (const AZStd::vector<char>& buffer = asset->GetBuffer(); !buffer.empty())
        {
            // Loads properties from XML file and populate the blackboard
            AZ::rapidxml::xml_document<char> doc;
            auto xml = AZStd::string(buffer.cbegin(), buffer.cend());
            doc.parse<0>(xml.data());

            if (const AZ::rapidxml::xml_node<char>* rootNode = doc.first_node("root"))
            {
                // AZ::rapidxml::xml_node<char>* treeModelNode = rootNode->first_node("TreeNodeModel");
                if (AZ::rapidxml::xml_node<char>* blackboardNode = rootNode->first_node("Blackboard"))
                {
                    Factory factory;
                    EBUS_EVENT_RESULT(factory, BehaveBehaviorTreeRequestBus, GetFactory);

                    if (const AZ::rapidxml::xml_attribute<char>* blackboardName = blackboardNode->first_attribute("name"))
                    {
                        _behaviorTreeComponent._btBlackboard.mName = blackboardName->value();
                    }

                    AZ::rapidxml::xml_node<char>* propertyNode;
                    while ((propertyNode = blackboardNode->first_node("Property")))
                    {
                        if (const AZ::rapidxml::xml_attribute<char>* name = propertyNode->first_attribute("name"))
                        {
                            const AZ::rapidxml::xml_attribute<char>* type = propertyNode->first_attribute("type");
                            auto valueType = "AZStd::string";

                            if (!type)
                            {
                                AZ_Warning(
                                    "BehaveAI [BehaviorTree]", false,
                                    "BehaviorTree Parse Error: Found a Property node (%s) without the \"type\" attribute. Assuming the "
                                    "type to be an AZStd::string.",
                                    name->value());
                            }
                            else
                            {
                                valueType = type->value();
                            }

                            const AZ::Uuid& valueTypeUuId = factory.GetRegistry()->GetPropertyUuid(valueType);
                            AZ_Error(
                                "BehaveAI [BehaviorTree]", !valueTypeUuId.IsNull(),
                                "The type \"%s\" is used in the behavior tree but was not registered. You will need to register the type "
                                "with Registry::RegisterProperty<T>(\"%s\") to avoid serialization errors.",
                                valueType, valueType);

                            bool needToCreateProperty;

                            if (BlackboardProperty* btProp = _behaviorTreeComponent._btBlackboard.GetProperty(name->value()); !btProp)
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
                                    if (const auto findIt = AZStd::find(
                                            _behaviorTreeComponent._btBlackboard.mProperties.begin(),
                                            _behaviorTreeComponent._btBlackboard.mProperties.end(), btProp);
                                        findIt != _behaviorTreeComponent._btBlackboard.mProperties.end())
                                    {
                                        delete *findIt;
                                        _behaviorTreeComponent._btBlackboard.mProperties.erase(findIt);
                                    }

                                    needToCreateProperty = true;
                                }
                            }

                            if (needToCreateProperty)
                            {
                                if (AZStd::unique_ptr<BlackboardProperty> prop = factory.CreateProperty(valueType, name->value()))
                                {
                                    if (const AZ::rapidxml::xml_attribute<char>* v = propertyNode->first_attribute("default"))
                                    {
                                        prop->SetValueFromString(v->value());
                                    }

                                    _behaviorTreeComponent._btBlackboard.mProperties.emplace_back(AZStd::move(prop.release()));
                                }
                            }

                            if (BlackboardProperty* prop = _behaviorTreeComponent._btBlackboard.GetProperty(name->value()))
                            {
                                prop->mVisibility = AZ::Edit::PropertyVisibility::ShowChildrenOnly;

                                if (const AZ::rapidxml::xml_attribute<char>* s = propertyNode->first_attribute("suffix"))
                                {
                                    prop->mSuffix = s->value();
                                }

                                if (const AZ::rapidxml::xml_attribute<char>* d = propertyNode->first_attribute("description"))
                                {
                                    prop->mDescription = d->value();
                                }

                                if (const AZ::rapidxml::xml_attribute<char>* p = propertyNode->first_attribute("private"))
                                {
                                    prop->mVisibility = BT::convertFromString<bool>(p->value())
                                        ? AZ::Edit::PropertyVisibility::Hide
                                        : AZ::Edit::PropertyVisibility::ShowChildrenOnly;
                                }

                                if (const AZ::rapidxml::xml_attribute<char>* p = propertyNode->first_attribute("order"))
                                {
                                    prop->mOrder = BT::convertFromString<float>(p->value());
                                }

                                ElementInfo ei;
                                ei.mEditData.m_name = CacheString(name->value());
                                ei.mEditData.m_description = prop->mDescription.c_str();
                                ei.mEditData.m_elementId = AZ::Edit::UIHandlers::Default;
                                ei.mSortOrder = prop->mOrder;

                                // Add the attributes to the map of default values
                                ei.mUuid = prop->GetDataTypeUuid();
                                ei.mIsAttributeOwner = true;
                                _dataElements.insert(AZStd::make_pair(prop->GetDataAddress(), ei));

                                // Also register to the script property itself, so friendly data can be displayed at its own level.
                                ei.mUuid = azrtti_typeid(prop);
                                ei.mIsAttributeOwner = false;
                                _dataElements.insert(AZStd::make_pair(prop, ei));

                                LoadAttribute(name->value(), ei.mEditData, prop, valueType);
                            }

                            blackboardNode->remove_node(propertyNode);
                        }
                        else
                        {
                            AZ_Error(
                                "BehaveAI [BehaviorTree]", false,
                                "BehaviorTree Parse Error: Found a Property node without the \"name\" attribute. This may lead to "
                                "errors during the behavior tree execution.");
                        }
                    }
                }
            }

            // Remove old properties to make the property grid always up to date
            RemovedOldProperties(_behaviorTreeComponent._btBlackboard);

            // Sort the list of property in alphabetical order
            SortProperties(_behaviorTreeComponent._btBlackboard);

            // Parses the list of provided properties in the blackboard
            ParseProperties(_behaviorTreeComponent._btBlackboard);
        }
    }

    void BehaviorTreeEditorComponent::ParseProperties([[maybe_unused]] const BehaviorTree::Blackboard::Blackboard& blackboard)
    {
    }

    void BehaviorTreeEditorComponent::RemovedOldProperties(BehaviorTree::Blackboard::Blackboard& blackboard)
    {
        for (auto it = blackboard.mProperties.begin(); it != blackboard.mProperties.end();)
        {
            if (const BlackboardProperty* prop = *it; _dataElements.find(prop->GetDataAddress()) == _dataElements.end())
            {
                delete prop;
                it = blackboard.mProperties.erase(it); // We did not find a data element, it must an old property
            }
            else
            {
                ++it;
            }
        }
    }

    void BehaviorTreeEditorComponent::SortProperties(BehaviorTree::Blackboard::Blackboard& blackboard)
    {
        // Sort properties
        AZStd::sort(
            blackboard.mProperties.begin(), blackboard.mProperties.end(),
            [&](const BlackboardProperty* lhs, const BlackboardProperty* rhs) -> bool
            {
                const auto lhsElementInfoPair = _dataElements.find(lhs);
                const auto rhsElementInfoPair = _dataElements.find(rhs);

                AZ_Assert(
                    lhsElementInfoPair != _dataElements.end() && rhsElementInfoPair != _dataElements.end(),
                    "We have blackboard properties that have do not have dataElements, this should not be possible!");

                if (lhsElementInfoPair->second.mSortOrder == FLT_MAX && rhsElementInfoPair->second.mSortOrder == FLT_MAX)
                {
                    // Use alphabetical sort
                    return lhs->mName < rhs->mName;
                }
                else
                {
                    // Sort based on the order defined by user
                    return lhsElementInfoPair->second.mSortOrder < rhsElementInfoPair->second.mSortOrder;
                }
            });
    }

    void BehaviorTreeEditorComponent::SetBehaviorTree(const AZ::Data::Asset<Assets::BehaviorTreeAsset>& tree)
    {
        _behaviorTreeAsset = tree;
        _behaviorTreeComponent.SetBehaviorTree(tree);

        _customName = "Behave AI - BehaviorTree";

        if (tree)
        {
            bool outcome = false;
            AZStd::string folderFoundIn;
            AZ::Data::AssetInfo assetInfo;

            AzToolsFramework::AssetSystemRequestBus::BroadcastResult(
                outcome, &AzToolsFramework::AssetSystemRequestBus::Events::GetSourceInfoBySourceUUID, _behaviorTreeAsset.GetId().m_guid,
                assetInfo, folderFoundIn);

            if (outcome)
            {
                AZStd::string name;
                AzFramework::StringFunc::Path::GetFileName(assetInfo.m_relativePath.c_str(), name);
                _customName += AZStd::string::format(" - %s", name.c_str());
            }
        }
    }

    AZ::u32 BehaviorTreeEditorComponent::BehaviorTreeHasChanged()
    {
        AZ::Data::AssetBus::Handler::BusDisconnect();

        // Only clear properties and data elements if the asset we're changing to is not the same one we already had set on our behavior
        // tree component The only time we shouldn't do this is when someone has set the same script on the component through the editor
        if (_behaviorTreeAsset != _behaviorTreeComponent.GetBehaviorTree())
        {
            _behaviorTreeComponent._btBlackboard.Clear();
            ClearDataElements();
        }

        if (_behaviorTreeAsset.GetId().IsValid())
        {
            SetBehaviorTree(_behaviorTreeAsset);

            AZ::Data::AssetBus::Handler::BusConnect(_behaviorTreeAsset.GetId());
            _behaviorTreeAsset.QueueLoad();
        }
        else
        {
            SetBehaviorTree();
        }

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }

    const AZ::Edit::ElementData* BehaviorTreeEditorComponent::GetScriptPropertyEditData(
        const void* handlerPtr, const void* elementPtr, const AZ::Uuid& elementType)
    {
        const auto* owner = static_cast<const BehaviorTreeEditorComponent*>(handlerPtr);
        return owner->GetDataElement(elementPtr, elementType);
    }

    void BehaviorTreeEditorComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            BehaviorTreeComponent::Reflect(rc);

            sc->Class<BehaviorTreeEditorComponent, EditorComponentBase>()
                ->Version(0)
                ->Field("Component", &BehaviorTreeEditorComponent::_behaviorTreeComponent)
                ->Field("Asset", &BehaviorTreeEditorComponent::_behaviorTreeAsset);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<BehaviorTreeEditorComponent>(
                      "BehaviorTree", "An implementation of behavior trees for O3DE, powered by BehaviorTree.CPP.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Behave AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BehaviorTreeEditorComponent::_customName)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, Assets::BehaviorTreeAssetHandler::GetAssetTypeStatic())
                    ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/SSBehaveAI_BehaviorTree.svg")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Editor/Icons/Viewport/SSBehaveAI_BehaviorTree.png")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaviorTreeEditorComponent::_behaviorTreeAsset, "BehaviorTree",
                        "The behavior tree to execute.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BehaviorTreeEditorComponent::BehaviorTreeHasChanged)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &BehaviorTreeEditorComponent::_behaviorTreeComponent, "BehaviorTree Properties",
                        "The behavior tree")
                    ->SetDynamicEditDataProvider(&BehaviorTreeEditorComponent::GetScriptPropertyEditData)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);

                ec->Class<BehaviorTreeComponent>(
                      "BehaviorTree", "An implementation of behavior trees for O3DE, powered by BehaviorTree.CPP.")
                    ->DataElement(0, &BehaviorTreeComponent::_btBlackboard, "Blackboard", "BehaviorTree properties.")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(0, &BehaviorTreeComponent::_behaviorTreeAsset, "Asset", "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Hide)
                    ->Attribute(
                        AZ::Edit::Attributes::SliceFlags,
                        AZ::Edit::SliceFlags::NotPushable) // Only the editor-component's script asset needs to be slice-pushable.
                    ;

                ec->Class<BehaviorTree::Blackboard::Blackboard>("BehaviorTree Blackboard", "The blackboard of a behavior tree.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "Blackboard's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BehaviorTree::Blackboard::Blackboard::mName)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(0, &BehaviorTree::Blackboard::Blackboard::mProperties, "Properties", "Properties in this blackboard.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);

                ec->Class<BlackboardProperty>("BehaviorTree Blackboard Property", "Base class for blackboard properties")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "BlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility);

                ec->Class<BlackboardPropertyBoolean>("BehaviorTree Property (bool)", "A blackboard boolean property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "BlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility)
                    ->DataElement(0, &BlackboardPropertyBoolean::mValue, "Value", "A boolean")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BlackboardProperty::mName)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &BlackboardProperty::mSuffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &BlackboardProperty::mDescription);

                ec->Class<BlackboardPropertyNumber>("BehaviorTree Blackboard Property (number)", "A blackboard number property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "BlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility)
                    ->DataElement(0, &BlackboardPropertyNumber::mValue, "Value", "A number")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BlackboardProperty::mName)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &BlackboardProperty::mSuffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &BlackboardProperty::mDescription);

                ec->Class<BlackboardPropertyString>("BehaviorTree Blackboard Property (string)", "A blackboard string property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "BlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility)
                    ->DataElement(0, &BlackboardPropertyString::mValue, "Value", "A string")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BlackboardProperty::mName)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &BlackboardProperty::mSuffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &BlackboardProperty::mDescription);

                ec->Class<BlackboardPropertyEntityRef>(
                      "BehaviorTree Blackboard Property (EntityRef)", "A blackboard entity reference property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "BlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility)
                    ->DataElement(0, &BlackboardPropertyEntityRef::mValue, "Value", "An entity reference")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BlackboardProperty::mName)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &BlackboardProperty::mSuffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &BlackboardProperty::mDescription);

                ec->Class<BlackboardPropertyVector2>("BehaviorTree Blackboard Property (Vector2)", "A blackboard Vector2 property")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "BlackboardProperty's class attributes.")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &BlackboardProperty::mVisibility)
                    ->DataElement(0, &BlackboardPropertyVector2::mValue, "Value", "An two dimensional vector")
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, &BlackboardProperty::mName)
                    ->Attribute(AZ::Edit::Attributes::Suffix, &BlackboardProperty::mSuffix)
                    ->Attribute(AZ::Edit::Attributes::DescriptionTextOverride, &BlackboardProperty::mDescription);
            }
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree
