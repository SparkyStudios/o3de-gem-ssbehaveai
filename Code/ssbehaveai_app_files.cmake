# Copyright (c) 2021-present Sparky Studios. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(FILES
    Editor/Application/precompiled.h

    Editor/Application/Application.cpp
    Editor/Application/Application.h

    Editor/Core/EditorBase.cpp
    Editor/Core/EditorBase.h
    Editor/Core/EditorFlowScene.cpp
    Editor/Core/EditorFlowScene.h
    Editor/Core/EditorFlowView.cpp
    Editor/Core/EditorFlowView.h
    Editor/Core/EditorUtils.cpp
    Editor/Core/EditorUtils.h

    Editor/Core/Models/SSBehaviorTreeNodeDataModel.cpp
    Editor/Core/Models/SSBehaviorTreeNodeDataModel.h
    Editor/Core/Models/SubTreeNodeModel.cpp
    Editor/Core/Models/SubTreeNodeModel.h

    Editor/Resources/icons.qrc
    Editor/Resources/SSBehaveAI_BehaviorTree.Editor.App.rc
    Editor/Resources/application-icon.ico

    Editor/Widgets/BlackboardProperties.cpp
    Editor/Widgets/BlackboardProperties.h
    Editor/Widgets/BlackboardProperties.ui
    Editor/Widgets/GraphicContainer.cpp
    Editor/Widgets/GraphicContainer.h
    Editor/Widgets/NodeProperties.cpp
    Editor/Widgets/NodeProperties.h
    Editor/Widgets/NodeProperties.ui
    Editor/Widgets/NodesSidePanel.cpp
    Editor/Widgets/NodesSidePanel.h
    Editor/Widgets/NodesSidePanel.ui
    Editor/Widgets/StatusBar.cpp
    Editor/Widgets/StatusBar.h
    Editor/Widgets/StatusBar.ui

    Editor/Windows/BlackboardPropertyDialog.cpp
    Editor/Windows/BlackboardPropertyDialog.h
    Editor/Windows/BlackboardPropertyDialog.ui
    Editor/Windows/MainWindow.cpp
    Editor/Windows/MainWindow.h

    Editor/main.cpp
)
