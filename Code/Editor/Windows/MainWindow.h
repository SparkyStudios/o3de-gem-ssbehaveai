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

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>

#include <AzCore/IO/Path/Path.h>

#include <AzQtComponents/Components/DockMainWindow.h>
#include <AzQtComponents/Components/FancyDocking.h>
#include <AzQtComponents/Components/StyledDockWidget.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Factory.h>

#include <QDomDocument>
#include <QMainWindow>
#include <QXmlStreamWriter>

#include <Editor/Widgets/BlackboardProperties.h>
#include <Editor/Widgets/GraphicContainer.h>
#include <Editor/Widgets/NodeProperties.h>
#include <Editor/Widgets/NodesSidePanel.h>
#include <Editor/Widgets/StatusBar.h>
#endif

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Windows
{
    class MainWindow : public AzQtComponents::DockMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent, const AZ::IO::PathView& projectPath, const AZ::IO::PathView& filePath);

        Widgets::GraphicContainer* CurrentTabInfo();
        Widgets::GraphicContainer* GetTabByName(const QString& name);
        void LockEditor(const bool locked) const;
        void LoadFromXML(const QString& xml);
        void SetCurrentNode(QtNodes::Node* node = nullptr);

        BehaviorTree::Core::Factory m_factory;

    public slots:
        void OnAutoArrange();
        void OnSceneChanged();
        void OnUndoInvoked();
        void OnRedoInvoked();
        void OnTabSetMainTree(int index);
        void OnTabCustomContextMenuRequested(const QPoint& pos);
        void OnActionClearTriggered(bool createMew);
        void OnPushUndo();
        void OnCreateAbsBehaviorTree(const Core::AbstractBehaviorTree& tree, const QString& btName, bool secondaryTabs = true);
        void OnActionNewTriggered();
        bool OnActionSaveTriggered();
        bool OnActionSaveAsTriggered();
        void OnActionLoadTriggered();
        void OnActionQuitTriggered();
        void OnAddToModelRegistry(const Core::NodeModel& model);
        void OnTabRenameRequested(int tabIndex, QString newName = QString());
        void OnTreeNodeEdited(const QString& prevId, const QString& newId);

    signals:
        void ValidTree();
        void InvalidTree();

    private:
        struct SavedState
        {
            QString mMainTree;
            QString mCurrentTabName;
            QTransform mViewTransform;
            QRectF mViewArea;
            std::map<QString, QByteArray> mJsonStates;

            bool operator==(const SavedState& other) const;
            bool operator!=(const SavedState& other) const
            {
                return !(*this == other);
            }
        };

        enum SubtreeExpandOption
        {
            SUBTREE_EXPAND,
            SUBTREE_COLLAPSE,
            SUBTREE_CHANGE,
            SUBTREE_REFRESH
        };

        void SetupMenu();
        Widgets::GraphicContainer* CreateTab(const QString& name, bool setActive = true);
        std::shared_ptr<QtNodes::DataModelRegistry> CreateDataModelRegistry();
        void LoadSavedStateFromJson(SavedState state);
        SavedState SaveCurrentState();
        void ClearUndoStacks();
        QString SaveToXml() const;
        QString XmlDocumentToString(const QDomDocument& document) const;
        void StreamElementAttributes(QXmlStreamWriter& stream, const QDomElement& element) const;
        void RecursivelySaveNodeCanonically(QXmlStreamWriter& stream, const QDomNode& parentNode) const;
        QtNodes::Node* SubTreeExpand(Widgets::GraphicContainer& container, QtNodes::Node& node, MainWindow::SubtreeExpandOption option);
        bool SaveFile(bool overwrite);
        bool OpenFile(const QString& filename);
        bool CheckDirty(const QString& message);

        AzQtComponents::FancyDocking* _fancyDocking;

        AzQtComponents::TabWidget* _tabWidget;
        Widgets::NodesSidePanel* _sidePanel;
        Widgets::StatusBar* _statusBar;
        Widgets::NodeProperties* _nodeProperties;
        Widgets::BlackboardProperties* _blackboardProperties;

        std::deque<SavedState> _undoStack;
        std::deque<SavedState> _redoStack;
        SavedState _currentState;

        std::shared_ptr<QtNodes::DataModelRegistry> _modelRegistry;
        std::map<QString, Widgets::GraphicContainer*> _tabInfo;
        QtNodes::PortLayout _currentLayout;
        Core::NodeModels _treeNodeModels;
        Core::BlackboardPropertyModels _blackboardModels;
        QString _mainTree;

        QString _openedFile;
        bool _dirtyFile;

        QString _projectPath;
    };

} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Windows
