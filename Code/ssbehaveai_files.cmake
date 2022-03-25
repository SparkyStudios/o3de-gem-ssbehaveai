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
    Include/SparkyStudios/AI/Behave/BehaviorTree/SSBehaviorTreeBus.h

    Include/SparkyStudios/AI/Behave/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h

    Include/SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeFactory.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeNode.h
    Include/SparkyStudios/AI/Behave/BehaviorTree/Core/SSBehaviorTreeRegistry.h

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

    Include/SparkyStudios/AI/Behave/Navigation/BehaveNavigationBus.h
    Include/SparkyStudios/AI/Behave/Navigation/BehaveNavigationMeshBus.h

    Source/BehaviorTree/Assets/SSBehaviorTreeAsset.h
    Source/BehaviorTree/Assets/SSBehaviorTreeAsset.cpp

    Source/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.cpp
    Source/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.cpp

    Source/BehaviorTree/Core/SSBehaviorTreeFactory.cpp
    Source/BehaviorTree/Core/SSBehaviorTreeNode.cpp
    Source/BehaviorTree/Core/SSBehaviorTreeRegistry.cpp

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

    Source/BehaviorTree/SSBehaviorTreeComponent.h
    Source/BehaviorTree/SSBehaviorTreeComponent.cpp
    Source/BehaviorTree/SSBehaviorTreeSystemComponent.cpp
    Source/BehaviorTree/SSBehaviorTreeSystemComponent.h
)
