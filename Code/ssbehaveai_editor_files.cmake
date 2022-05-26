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
    Source/StdAfx.cpp
    Source/StdAfx.h

    Source/BehaviorTree/Assets/BehaviorTreeAssetHandler.cpp
    Source/BehaviorTree/Assets/BehaviorTreeAssetHandler.h

    Source/BehaviorTree/BehaviorTreeEditorComponent.cpp
    Source/BehaviorTree/BehaviorTreeEditorComponent.h
    Source/BehaviorTree/BehaviorTreeEditorSystemComponent.cpp
    Source/BehaviorTree/BehaviorTreeEditorSystemComponent.h

    Source/Navigation/BehaveNavigationEditorSystemComponent.h
    Source/Navigation/BehaveNavigationEditorSystemComponent.cpp

    Source/Navigation/Components/DynamicNavigationMeshEditorComponent.h
    Source/Navigation/Components/DynamicNavigationMeshEditorComponent.cpp
    Source/Navigation/Components/NavigationMeshAreaEditorComponent.h
    Source/Navigation/Components/NavigationMeshAreaEditorComponent.cpp
    Source/Navigation/Components/WalkableEditorComponent.h
    Source/Navigation/Components/WalkableEditorComponent.cpp

    Source/Navigation/Utils/Constants.h
    Source/Navigation/Utils/RecastNavMeshDebugDraw.h
    Source/Navigation/Utils/RecastNavMeshDebugDraw.cpp

    # Recast Navigation SDK
    # ==============================
    ../External/RecastNavigation/DebugUtils/Include/DebugDraw.h
    ../External/RecastNavigation/DebugUtils/Include/DetourDebugDraw.h
    ../External/RecastNavigation/DebugUtils/Include/RecastDebugDraw.h
    ../External/RecastNavigation/DebugUtils/Include/RecastDump.h

    ../External/RecastNavigation/DebugUtils/Source/DebugDraw.cpp
    ../External/RecastNavigation/DebugUtils/Source/DetourDebugDraw.cpp
    ../External/RecastNavigation/DebugUtils/Source/RecastDebugDraw.cpp
    ../External/RecastNavigation/DebugUtils/Source/RecastDump.cpp
    # ==============================
)
