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

#include <BehaviorTree/BehaviorTreeEditorComponent.h>
#include <BehaviorTree/BehaviorTreeEditorSystemComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Utils/Utils.h>

#include <QApplication>
#include <QMenu>
#include <QProcess>

#include <AzFramework/API/ApplicationAPI.h>

#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/API/ViewPaneOptions.h>
#include <AzToolsFramework/AssetBrowser/Entries/SourceAssetBrowserEntry.h>
#include <AzToolsFramework/UI/PropertyEditor/GenericComboBoxCtrl.h>
#include <AzToolsFramework/Viewport/ActionBus.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    namespace AssetHelpers
    {
        // Given the AssetId to the asset, attempt to get the AssetInfo
        static AZ::Data::AssetInfo GetAssetInfo(AZ::Data::AssetId assetId)
        {
            AZ::Data::AssetInfo assetInfo;
            EBUS_EVENT_RESULT(assetInfo, AZ::Data::AssetCatalogRequestBus, GetAssetInfoById, assetId);
            return assetInfo;
        }

        // Find the AssetType for a given asset
        static AZ::Data::AssetType GetAssetType(AZ::Data::AssetId assetId)
        {
            AZ::Data::AssetInfo assetInfo = GetAssetInfo(assetId);
            return assetInfo.m_assetType;
        }

        // Get AssetInfo from the AssetSystem as opposed to the catalog
        static AZ::Data::AssetInfo GetAssetInfo(AZ::Data::AssetId assetId, AZStd::string& rootFilePath)
        {
            AZ::Data::AssetInfo assetInfo;
            const AZStd::string platformName = "";
            EBUS_EVENT(
                AzToolsFramework::AssetSystemRequestBus, GetAssetInfoById, assetId, GetAssetType(assetId), platformName, assetInfo,
                rootFilePath);
            return assetInfo;
        }
    } // namespace AssetHelpers

    BehaviorTreeEditorSystemComponent::BehaviorTreeEditorSystemComponent()
    {
        _openEditorAction = nullptr;
        _assetHandler = aznew Assets::BehaviorTreeAssetHandler();
    }

    BehaviorTreeEditorSystemComponent::~BehaviorTreeEditorSystemComponent()
    {
        delete _assetHandler;
    }

    void BehaviorTreeEditorSystemComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* const sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<BehaviorTreeEditorSystemComponent, BehaviorTreeSystemComponent>()->Version(0);
        }
    }

    void BehaviorTreeEditorSystemComponent::OnApplicationAboutToStop()
    {
        // TeardownThumbnails();
    }

    void BehaviorTreeEditorSystemComponent::OnPopulateToolMenuItems()
    {
        if (!_openEditorAction)
        {
            _openEditorAction = new QAction("Behave AI - BehaviorTree Editor");
            _openEditorAction->setCheckable(false);
            _openEditorAction->setChecked(false);
            _openEditorAction->setIcon(QIcon(":/Menu/ss_behaviortree_editor.svg"));
            QObject::connect(
                _openEditorAction, &QAction::triggered, _openEditorAction,
                [this]()
                {
                    OpenEditor();
                });

            EBUS_EVENT(AzToolsFramework::EditorMenuRequestBus, AddMenuAction, "ToolMenu", _openEditorAction, true);
        }
    }

    void BehaviorTreeEditorSystemComponent::OnResetToolMenuItems()
    {
        if (_openEditorAction)
        {
            delete _openEditorAction;
            _openEditorAction = nullptr;
        }
    }

    void BehaviorTreeEditorSystemComponent::PopulateEditorGlobalContextMenu_SliceSection(QMenu* menu, const AZ::Vector2& point, int flags)
    {
        AZ_UNUSED(point);
        AZ_UNUSED(flags);

        if (!menu)
        {
            return;
        }

        AzToolsFramework::EntityIdList entitiesWithBehaviorTree;

        AzToolsFramework::EntityIdList selectedEntities;
        AzToolsFramework::EntityIdList highlightedEntities;

        EBUS_EVENT_RESULT(selectedEntities, AzToolsFramework::ToolsApplicationRequests::Bus, GetSelectedEntities);

        FilterForBehaviorTreeEnabledEntities(selectedEntities, entitiesWithBehaviorTree);

        EBUS_EVENT_RESULT(highlightedEntities, AzToolsFramework::ToolsApplicationRequests::Bus, GetHighlightedEntities);

        FilterForBehaviorTreeEnabledEntities(highlightedEntities, entitiesWithBehaviorTree);

        if (!entitiesWithBehaviorTree.empty())
        {
            QMenu* ssBehaviorTreeMenu = nullptr;
            QAction* action = nullptr;

            // For entities with behavior tree component, create a context menu to open any existing behavior trees within each selected
            // entity.
            for (const AZ::EntityId& entityId : entitiesWithBehaviorTree)
            {
                if (!ssBehaviorTreeMenu)
                {
                    if (!menu->children().empty())
                    {
                        menu->addSeparator();
                    }

                    ssBehaviorTreeMenu = menu->addMenu(QObject::tr("Edit BehaviorTree"));
                    ssBehaviorTreeMenu->setEnabled(false);
                    menu->addSeparator();
                }

                AZ::Entity* entity = nullptr;
                EBUS_EVENT_RESULT(entity, AZ::ComponentApplicationBus, FindEntity, entityId);

                if (entity)
                {
                    AZ::EBusAggregateResults<AZ::Data::AssetId> assetIds;
                    EBUS_EVENT_ID_RESULT(assetIds, entity->GetId(), BehaviorTreeEditorComponentRequestBus, GetAssetId);

                    if (!assetIds.values.empty())
                    {
                        QMenu* entityMenu = ssBehaviorTreeMenu;
                        if (entitiesWithBehaviorTree.size() > 1)
                        {
                            ssBehaviorTreeMenu->setEnabled(true);
                            entityMenu = ssBehaviorTreeMenu->addMenu(entity->GetName().c_str());
                            entityMenu->setEnabled(false);
                        }

                        AZStd::unordered_set<AZ::Data::AssetId> usedIds;

                        for (const auto& assetId : assetIds.values)
                        {
                            if (!assetId.IsValid() || usedIds.count(assetId) != 0)
                            {
                                continue;
                            }

                            entityMenu->setEnabled(true);

                            usedIds.insert(assetId);

                            AZStd::string rootPath;
                            AZ::Data::AssetInfo assetInfo = AssetHelpers::GetAssetInfo(assetId, rootPath);

                            AZStd::string displayName;
                            AZ::StringFunc::Path::GetFileName(assetInfo.m_relativePath.c_str(), displayName);

                            action = entityMenu->addAction(QString("%1").arg(QString(displayName.c_str())));

                            AZ_Printf("BehaveAI [BehaviorTree]", "Adding menu item for asset");
                            QObject::connect(
                                action, &QAction::triggered,
                                [this, rootPath]
                                {
                                    OpenEditor(rootPath.c_str());
                                });
                        }
                    }
                }
            }
        }
    }

    void BehaviorTreeEditorSystemComponent::AddSourceFileOpeners(
        const char* fullSourceFileName, const AZ::Uuid& sourceUUID, AzToolsFramework::AssetBrowser::SourceFileOpenerList& openers)
    {
        using namespace AzToolsFramework;

        // Get asset group to support a variety of file extensions
        const AzToolsFramework::AssetBrowser::SourceAssetBrowserEntry* fullDetails =
            AzToolsFramework::AssetBrowser::SourceAssetBrowserEntry::GetSourceByUuid(sourceUUID);
        if (!fullDetails)
        {
            return;
        }
        QString assetGroup;
        AZ::AssetTypeInfoBus::EventResult(assetGroup, fullDetails->GetPrimaryAssetType(), &AZ::AssetTypeInfo::GetGroup);

        if (AZStd::wildcard_match("*.bhbtree", fullSourceFileName))
        {
            AZStd::string fullName(fullSourceFileName);
            openers.push_back({ "SparkyStudios_AI_Behave_BehaviorTree_Editor", "Open in BehaviorTree Editor...",
                                QIcon(":/Menu/ss_behaviortree_editor.svg"),
                                [&](const char* fullSourceFileNameInCallback, const AZ::Uuid& /*sourceUUID*/)
                                {
                                    OpenEditor(fullSourceFileNameInCallback);
                                } });
        }
    }

    const AzToolsFramework::AssetBrowser::ProductAssetBrowserEntry* BehaviorTreeEditorSystemComponent::GetProductFromBrowserEntry(
        AzToolsFramework::AssetBrowser::AssetBrowserEntry* entry) const
    {
        if (!entry)
        {
            return nullptr;
        }

        using namespace AzToolsFramework::AssetBrowser;

        const ProductAssetBrowserEntry* product = nullptr;

        if (entry->GetEntryType() == AssetBrowserEntry::AssetEntryType::Source)
        {
            // See if our first entry has a product of the appropriate type
            AZStd::vector<const ProductAssetBrowserEntry*> productChildren;
            entry->GetChildren(productChildren);

            if (auto entryIt = AZStd::find_if(
                    productChildren.begin(), productChildren.end(),
                    [this](const ProductAssetBrowserEntry* entry) -> bool
                    {
                        return IsBehaviorTreeAssetType(entry->GetAssetType());
                    });
                entryIt != productChildren.end())
            {
                product = *entryIt;
            }
        }
        else if (entry->GetEntryType() == AssetBrowserEntry::AssetEntryType::Product)
        {
            if (const auto* productCast = azrtti_cast<const ProductAssetBrowserEntry*>(entry);
                productCast && IsBehaviorTreeAssetType(productCast->GetAssetType()))
            {
                // Entry is the right type, proceed
                product = productCast;
            }
        }

        return product;
    }

    bool BehaviorTreeEditorSystemComponent::IsBehaviorTreeAssetType(const AZ::Data::AssetType& type) const
    {
        return (type == AZ::AzTypeInfo<Assets::BehaviorTreeAsset>::Uuid());
    }

    void BehaviorTreeEditorSystemComponent::FilterForBehaviorTreeEnabledEntities(
        AzToolsFramework::EntityIdList& sourceList, AzToolsFramework::EntityIdList& targetList)
    {
        for (const AZ::EntityId& entityId : sourceList)
        {
            if (entityId.IsValid())
            {
                if (BehaviorTreeEditorComponentRequestBus::FindFirstHandler(entityId))
                {
                    if (targetList.end() == AZStd::find(targetList.begin(), targetList.end(), entityId))
                    {
                        targetList.push_back(entityId);
                    }
                }
            }
        }
    }

    void BehaviorTreeEditorSystemComponent::OpenEditor(const char* file)
    {
        AZStd::string args = "";
        if (file && strlen(file) > 0)
        {
            args = AZStd::string::format("\"%s\"", file);
        }

        const AZ::IO::FixedMaxPathString engineRoot(AZ::Utils::GetEnginePath());
        AZ_Assert(!engineRoot.empty(), "Unable to get the engine root.");

        AZ::IO::FixedMaxPathString exePath(
            SS_PROJECT_RUNTIME_OUTPUT_DIRECTORY AZ_CORRECT_FILESYSTEM_SEPARATOR_STRING AZ_BUILD_CONFIGURATION_TYPE);
        const AZ::IO::FixedMaxPathString projectPath(AZ::Utils::GetProjectPath());

        const AZStd::string process = AZStd::string::format(
            "\"%.*s" AZ_CORRECT_FILESYSTEM_SEPARATOR_STRING "BehaviorTree.Editor"
#if defined(AZ_PLATFORM_WINDOWS)
            ".exe"
#endif
            "\"",
            aznumeric_cast<int>(exePath.size()), exePath.data());

        const AZStd::string processArgs =
            AZStd::string::format(R"(%s --engine-path "%s" --project-path "%s")", args.c_str(), engineRoot.c_str(), projectPath.c_str());

        StartProcessDetached(process.c_str(), processArgs.c_str());
    }

    void BehaviorTreeEditorSystemComponent::StartProcessDetached(const char* process, const char* args)
    {
        // Build the arguments as a QStringList
        AZStd::vector<AZStd::string> tokens;

        AZStd::string currentStr(args);
        AZStd::size_t firstQuotePos = AZStd::string::npos;
        AZStd::size_t secondQuotePos = 0;
        AZStd::size_t pos = 0;

        while (!currentStr.empty())
        {
            firstQuotePos = currentStr.find_first_of('\"');
            pos = currentStr.find_first_of(" ");

            if ((firstQuotePos != AZStd::string::npos) && (firstQuotePos < pos || pos == AZStd::string::npos))
            {
                secondQuotePos = currentStr.find_first_of('\"', firstQuotePos + 1);
                if (secondQuotePos == AZStd::string::npos)
                {
                    AZ_Warning("StartProcessDetached", false, "String tokenize failed, no matching \" found.");
                    return;
                }

                AZStd::string newElement(AZStd::string(currentStr.data() + (firstQuotePos + 1), (secondQuotePos - 1)));
                tokens.push_back(newElement);

                currentStr = currentStr.substr(secondQuotePos + 1);

                firstQuotePos = AZStd::string::npos;
                secondQuotePos = 0;
                continue;
            }
            else
            {
                if (pos != AZStd::string::npos)
                {
                    AZStd::string newElement(AZStd::string(currentStr.data() + 0, pos));
                    tokens.push_back(newElement);
                    currentStr = currentStr.substr(pos + 1);
                }
                else
                {
                    tokens.push_back(AZStd::string(currentStr));
                    break;
                }
            }
        }

        QStringList argsList;
        for (const auto& arg : tokens)
        {
            argsList.push_back(QString(arg.c_str()));
        }

        // Launch the process
        const bool startDetachedReturn = QProcess::startDetached(process, argsList, QCoreApplication::applicationDirPath());
        AZ_Warning("StartProcessDetached", startDetachedReturn, "Failed to start process:%s args:%s", process, args);
    }

    void BehaviorTreeEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("BehaveAI_BehaviorTreeEditorService"));
    }

    void BehaviorTreeEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("BehaveAI_BehaviorTreeEditorService"));
    }

    void BehaviorTreeEditorSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
        required.push_back(AZ_CRC_CE("AssetCatalogService"));
    }

    void BehaviorTreeEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void BehaviorTreeEditorSystemComponent::Activate()
    {
        if (AZ::Data::AssetManager::IsReady())
        {
            AZ::Data::AssetManager::Instance().RegisterHandler(_assetHandler, _assetHandler->GetAssetType());
        }

        // Register with the Asset Catalog
        EBUS_EVENT(AZ::Data::AssetCatalogRequestBus, EnableCatalogForAsset, _assetHandler->GetAssetType());
        EBUS_EVENT(AZ::Data::AssetCatalogRequestBus, AddExtension, _assetHandler->GetExtension().c_str());

        BaseSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
        AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler::BusConnect();

        AzFramework::ApplicationLifecycleEvents::Bus::Handler::BusConnect();
        AzToolsFramework::EditorMenuNotificationBus::Handler::BusConnect();
    }

    void BehaviorTreeEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorMenuNotificationBus::Handler::BusDisconnect();
        AzFramework::ApplicationLifecycleEvents::Bus::Handler::BusDisconnect();

        AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler::BusDisconnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        BaseSystemComponent::Deactivate();

        if (AZ::Data::AssetManager::IsReady())
        {
            AZ::Data::AssetManager::Instance().UnregisterHandler(_assetHandler);
        }

        if (_openEditorAction)
        {
            delete _openEditorAction;
            _openEditorAction = nullptr;
        }
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree
