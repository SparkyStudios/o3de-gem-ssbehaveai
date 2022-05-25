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

    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/BehaveBehaviorTreeBus.h

    Include/SparkyStudios/AI/Behave/BehaviorTree/Blackboard/Blackboard.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Blackboard/BlackboardProperty.h

    Include/SparkyStudios/AI/Behave/BehaviorTree/Core/Factory.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Core/Node.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Core/Registry.h

    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterStringNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterStringNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendInTimeNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendOutTimeNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetLoopMotionNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetMotionNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetPlaySpeedNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetPlayTimeNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionPlayNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendInTimeNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetLoopMotionNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetMirrorMotionNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetMotionNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetPlaySpeedNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetPlayTimeNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetRetargetMotionNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetReverseMotionNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Common/DebugMessageNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Common/WaitNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.h

    Include/SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshArea.h
    Include/SparkyStudios/AI/Behave/Navigation/BehaveNavigationBus.h
    Include/SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h
    Include/SparkyStudios/AI/Behave/Navigation/IBehaveNavigationMesh.h

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

    Source/Navigation/Assets/BehaveNavigationAgentAsset.h
    Source/Navigation/Assets/BehaveNavigationAgentAsset.cpp
    Source/Navigation/Assets/BehaveNavigationMeshAreaAsset.h
    Source/Navigation/Assets/BehaveNavigationMeshAreaAsset.cpp
    Source/Navigation/Assets/BehaveNavigationMeshSettingsAsset.h
    Source/Navigation/Assets/BehaveNavigationMeshSettingsAsset.cpp

    Source/Navigation/BehaveNavigationMeshArea.cpp
    Source/Navigation/BehaveNavigationMeshAreaProviderRequestBus.h
    Source/Navigation/BehaveNavigationSystemComponent.h
    Source/Navigation/BehaveNavigationSystemComponent.cpp

    Source/Navigation/Components/DynamicNavigationMeshComponent.h
    Source/Navigation/Components/DynamicNavigationMeshComponent.cpp
    Source/Navigation/Components/NavigationMeshAreaComponent.h
    Source/Navigation/Components/NavigationMeshAreaComponent.cpp
    Source/Navigation/Components/WalkableComponent.h
    Source/Navigation/Components/WalkableComponent.cpp

    Source/Navigation/Utils/RecastMath.h
    Source/Navigation/Utils/RecastMath.cpp
    Source/Navigation/Utils/RecastSmartPointer.h
    Source/Navigation/Utils/RecastNavigationMesh.h
    Source/Navigation/Utils/RecastNavigationMesh.cpp

    # Recast Navigation SDK
    # ==============================
    ../External/RecastNavigation/Recast/Include/RecastAlloc.h
    ../External/RecastNavigation/Recast/Include/RecastAssert.h
    ../External/RecastNavigation/Recast/Include/Recast.h

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

    ../External/RecastNavigation/Detour/Include/DetourAlloc.h
    ../External/RecastNavigation/Detour/Include/DetourAssert.h
    ../External/RecastNavigation/Detour/Include/DetourCommon.h
    ../External/RecastNavigation/Detour/Include/DetourMath.h
    ../External/RecastNavigation/Detour/Include/DetourNavMesh.h
    ../External/RecastNavigation/Detour/Include/DetourNavMeshBuilder.h
    ../External/RecastNavigation/Detour/Include/DetourNavMeshQuery.h
    ../External/RecastNavigation/Detour/Include/DetourNode.h
    ../External/RecastNavigation/Detour/Include/DetourStatus.h

    ../External/RecastNavigation/Detour/Source/DetourAlloc.cpp
    ../External/RecastNavigation/Detour/Source/DetourAssert.cpp
    ../External/RecastNavigation/Detour/Source/DetourCommon.cpp
    ../External/RecastNavigation/Detour/Source/DetourNavMesh.cpp
    ../External/RecastNavigation/Detour/Source/DetourNavMeshBuilder.cpp
    ../External/RecastNavigation/Detour/Source/DetourNavMeshQuery.cpp
    ../External/RecastNavigation/Detour/Source/DetourNode.cpp

    ../External/RecastNavigation/DetourCrowd/Include/DetourCrowd.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourLocalBoundary.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourObstacleAvoidance.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourPathCorridor.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourPathQueue.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourProximityGrid.h

    ../External/RecastNavigation/DetourCrowd/Source/DetourCrowd.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourLocalBoundary.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourObstacleAvoidance.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourPathCorridor.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourPathQueue.cpp
    ../External/RecastNavigation/DetourCrowd/Source/DetourProximityGrid.cpp

    ../External/RecastNavigation/DetourTileCache/Include/DetourTileCache.h
    ../External/RecastNavigation/DetourTileCache/Include/DetourTileCacheBuilder.h

    ../External/RecastNavigation/DetourTileCache/Source/DetourTileCache.cpp
    ../External/RecastNavigation/DetourTileCache/Source/DetourTileCacheBuilder.cpp
    # ==============================
)
