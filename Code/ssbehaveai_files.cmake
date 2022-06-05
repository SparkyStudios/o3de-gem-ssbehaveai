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

    Source/BehaviorTree/Assets/BehaviorTreeAsset.h
    Source/BehaviorTree/Assets/BehaviorTreeAsset.cpp

    Source/BehaviorTree/Blackboard/Blackboard.cpp
    Source/BehaviorTree/Blackboard/BlackboardProperty.cpp

    Source/BehaviorTree/Core/Factory.cpp
    Source/BehaviorTree/Core/Node.cpp
    Source/BehaviorTree/Core/Registry.cpp

    Source/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterStringNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterStringNode.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.cpp
    Source/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendInTimeNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendOutTimeNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionGetLoopMotionNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionGetMotionNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionGetPlaySpeedNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionGetPlayTimeNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionPlayNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendInTimeNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetLoopMotionNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetMirrorMotionNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetMotionNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetPlaySpeedNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetPlayTimeNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetRetargetMotionNode.cpp
    Source/BehaviorTree/Nodes/Animation/SimpleMotionSetReverseMotionNode.cpp
    Source/BehaviorTree/Nodes/Common/DebugMessageNode.cpp
    Source/BehaviorTree/Nodes/Common/WaitNode.cpp
    Source/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.cpp

    Source/BehaviorTree/BehaviorTreeComponent.h
    Source/BehaviorTree/BehaviorTreeComponent.cpp
    Source/BehaviorTree/BehaviorTreeSystemComponent.cpp
    Source/BehaviorTree/BehaviorTreeSystemComponent.h

    Source/Navigation/Assets/NavigationAgentsAsset.h
    Source/Navigation/Assets/NavigationAgentsAsset.cpp
    Source/Navigation/Assets/NavigationAreasAsset.h
    Source/Navigation/Assets/NavigationAreasAsset.cpp
    Source/Navigation/Assets/NavigationMeshSettingsAsset.h
    Source/Navigation/Assets/NavigationMeshSettingsAsset.cpp

    Source/Navigation/NavigationAgent.cpp
    Source/Navigation/NavigationArea.cpp
    Source/Navigation/NavigationAreaProviderRequestBus.h
    Source/Navigation/NavigationAgentProviderRequestBus.h
    Source/Navigation/NavigationSystemComponent.h
    Source/Navigation/NavigationSystemComponent.cpp
    Source/Navigation/OffMeshConnection.cpp

    Source/Navigation/Components/DynamicNavigationMeshComponent.h
    Source/Navigation/Components/DynamicNavigationMeshComponent.cpp
    Source/Navigation/Components/NavigationAreaComponent.h
    Source/Navigation/Components/NavigationAreaComponent.cpp
    Source/Navigation/Components/WalkableComponent.h
    Source/Navigation/Components/WalkableComponent.cpp

    Source/Navigation/Utils/RecastMath.h
    Source/Navigation/Utils/RecastMath.cpp
    Source/Navigation/Utils/RecastSmartPointer.h
    Source/Navigation/Utils/RecastNavigationMesh.h
    Source/Navigation/Utils/RecastNavigationMesh.cpp
    Source/Navigation/Utils/RecastChunkedGeometry.h
    Source/Navigation/Utils/RecastChunkedGeometry.cpp

    # Recast Navigation SDK
    # ==============================
    ../External/RecastNavigation/Recast/Source/Recast.cpp
    ../External/RecastNavigation/Recast/Source/RecastAlloc.cpp
    ../External/RecastNavigation/Recast/Source/RecastArea.cpp
    ../External/RecastNavigation/Recast/Source/RecastAssert.cpp
    ../External/RecastNavigation/Recast/Source/RecastContour.cpp
    ../External/RecastNavigation/Recast/Source/RecastFilter.cpp
    ../External/RecastNavigation/Recast/Source/RecastLayers.cpp
    ../External/RecastNavigation/Recast/Source/RecastMesh.cpp
    ../External/RecastNavigation/Recast/Source/RecastMeshDetail.cpp
    ../External/RecastNavigation/Recast/Source/RecastRasterization.cpp
    ../External/RecastNavigation/Recast/Source/RecastRegion.cpp

    ../External/RecastNavigation/Detour/Source/DetourAlloc.cpp
    ../External/RecastNavigation/Detour/Source/DetourAssert.cpp
    ../External/RecastNavigation/Detour/Source/DetourCommon.cpp
    ../External/RecastNavigation/Detour/Source/DetourNavMesh.cpp
    ../External/RecastNavigation/Detour/Source/DetourNavMeshBuilder.cpp
    ../External/RecastNavigation/Detour/Source/DetourNavMeshQuery.cpp
    ../External/RecastNavigation/Detour/Source/DetourNode.cpp

    ../External/RecastNavigation/DetourCrowd/Source/DetourCrowd.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourLocalBoundary.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourObstacleAvoidance.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourPathCorridor.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourPathQueue.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourProximityGrid.cpp

    ../External/RecastNavigation/DetourTileCache/Source/DetourTileCache.cpp
    ../External/RecastNavigation/DetourTileCache/Source/DetourTileCacheBuilder.cpp
    # ==============================
)
