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

#include <SSBehaviorTreeSystemComponent.h>

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

#include <Assets/SSBehaviorTreeAssetHandler.h>

#include <Editor/Windows/MainWindow.h>

namespace SparkyStudios::AI::Behave::BehaviorTree
{
    /// System component for SSBehaviorTree editor
    class SSBehaviorTreeEditorSystemComponent
        : public SSBehaviorTreeSystemComponent
        , private AzToolsFramework::EditorEvents::Bus::Handler
        , private AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler
        , private AzFramework::ApplicationLifecycleEvents::Bus::Handler
        , public AzToolsFramework::EditorMenuNotificationBus::Handler
    {
        using BaseSystemComponent = SSBehaviorTreeSystemComponent;

    public:
        AZ_COMPONENT(SSBehaviorTreeEditorSystemComponent, "{c6d81d4a-51ca-4c74-9d0d-96f813233c74}", BaseSystemComponent);
        static void Reflect(AZ::ReflectContext* context);

        // The name of the view pane displaying the BehaviorTree Editor.
        static constexpr const char* const ViewPaneName = "BehaviorTree Editor";

        SSBehaviorTreeEditorSystemComponent();
        ~SSBehaviorTreeEditorSystemComponent();

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
        bool IsSSBTAssetType(const AZ::Data::AssetType& type) const;
        void FilterForBehaviorTreeEnabledEntities(AzToolsFramework::EntityIdList& sourceList, AzToolsFramework::EntityIdList& targetList);

        ////////////////////////////////////////////////////////////////////////
        // AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler
        void AddSourceFileOpeners(
            const char* fullSourceFileName,
            const AZ::Uuid& sourceUUID,
            AzToolsFramework::AssetBrowser::SourceFileOpenerList& openers) override;
        ////////////////////////////////////////////////////////////////////////

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

        Assets::SSBehaviorTreeAssetHandler* m_assetHandler;
        QAction* m_openEditorAction;
    };
} // namespace SparkyStudios::AI::Behave::BehaviorTree
