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

    Include/SparkyStudios/AI/Behave/Navigation/NavigationArea.h
    Include/SparkyStudios/AI/Behave/Navigation/NavigationAgent.h
    Include/SparkyStudios/AI/Behave/Navigation/NavigationMeshBus.h
    Include/SparkyStudios/AI/Behave/Navigation/INavigationMesh.h
    Include/SparkyStudios/AI/Behave/Navigation/OffMeshConnection.h

    # Recast Navigation SDK
    # ==============================
    ../External/RecastNavigation/Recast/Include/RecastAlloc.h
    ../External/RecastNavigation/Recast/Include/RecastAssert.h
    ../External/RecastNavigation/Recast/Include/Recast.h

    ../External/RecastNavigation/Detour/Include/DetourAlloc.h
    ../External/RecastNavigation/Detour/Include/DetourAssert.h
    ../External/RecastNavigation/Detour/Include/DetourCommon.h
    ../External/RecastNavigation/Detour/Include/DetourMath.h
    ../External/RecastNavigation/Detour/Include/DetourNavMesh.h
    ../External/RecastNavigation/Detour/Include/DetourNavMeshBuilder.h
    ../External/RecastNavigation/Detour/Include/DetourNavMeshQuery.h
    ../External/RecastNavigation/Detour/Include/DetourNode.h
    ../External/RecastNavigation/Detour/Include/DetourStatus.h

    ../External/RecastNavigation/DetourCrowd/Include/DetourCrowd.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourLocalBoundary.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourObstacleAvoidance.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourPathCorridor.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourPathQueue.h
    ../External/RecastNavigation/DetourCrowd/Include/DetourProximityGrid.h

    ../External/RecastNavigation/DetourTileCache/Include/DetourTileCache.h
    ../External/RecastNavigation/DetourTileCache/Include/DetourTileCacheBuilder.h
    # ==============================
)
