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

#include <BehaviorTree/BehaviorTreeSystemComponent.h>

#include <QAction>

#include <AzFramework/Application/Application.h>

#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/AssetBrowser/AssetBrowserBus.h>
#include <AzToolsFramework/AssetBrowser/Entries/AssetBrowserEntry.h>
#include <AzToolsFramework/AssetBrowser/Entries/ProductAssetBrowserEntry.h>
#include <AzToolsFramework/Entity/EditorEntityContextBus.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#include <AzToolsFramework/ToolsComponents/EditorVisibilityBus.h>
#include <AzToolsFramework/Viewport/ActionBus.h>

#include <BehaviorTree/Assets/BehaviorTreeAssetHandler.h>

#include <Editor/Windows/MainWindow.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    /// System component for Behave's BehaviorTree editor
    class BehaviorTreeEditorSystemComponent
        : public BehaviorTreeSystemComponent
        , public AzToolsFramework::EditorMenuNotificationBus::Handler
        , private AzToolsFramework::EditorEvents::Bus::Handler
        , private AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler
        , private AzFramework::ApplicationLifecycleEvents::Bus::Handler
    {
        using BaseSystemComponent = BehaviorTreeSystemComponent;

    public:
        AZ_COMPONENT(BehaviorTreeEditorSystemComponent, "{C6D81D4A-51CA-4C74-9D0D-96F813233C74}", BaseSystemComponent);
        static void Reflect(AZ::ReflectContext* rc);

        // The name of the view pane displaying the BehaviorTree Editor.
        static constexpr const char* const ViewPaneName = "BehaviorTree Editor";

        BehaviorTreeEditorSystemComponent();
        ~BehaviorTreeEditorSystemComponent();

    protected:
        void PopulateEditorGlobalContextMenu_SliceSection(QMenu* menu, const AZ::Vector2& point, int flags) override;

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        void OnApplicationAboutToStop() override;

        void OnPopulateToolMenuItems() override;
        void OnResetToolMenuItems() override;

        const AzToolsFramework::AssetBrowser::ProductAssetBrowserEntry* GetProductFromBrowserEntry(
            AzToolsFramework::AssetBrowser::AssetBrowserEntry* entry) const;
        bool IsBehaviorTreeAssetType(const AZ::Data::AssetType& type) const;
        void FilterForBehaviorTreeEnabledEntities(AzToolsFramework::EntityIdList& sourceList, AzToolsFramework::EntityIdList& targetList);

        // AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler
        void AddSourceFileOpeners(
            const char* fullSourceFileName,
            const AZ::Uuid& sourceUUID,
            AzToolsFramework::AssetBrowser::SourceFileOpenerList& openers) override;

        void StartProcessDetached(const char* process, const char* args);

        /**
         * @brief Launches the behavior tree editor.
         *
         * @param file The path to the behavior tree file to load.
         */
        void OpenEditor(const char* file = nullptr);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        Assets::BehaviorTreeAssetHandler* _assetHandler;
        QAction* _openEditorAction;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree
