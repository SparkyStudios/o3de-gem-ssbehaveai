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

    Include/SparkyStudios/AI/Behave/Nodes.h
    Include/SparkyStudios/AI/Behave/SSBehaviorTreeBus.h

    Include/SparkyStudios/AI/Behave/Blackboard/SSBehaviorTreeBlackboard.h
    Include/SparkyStudios/AI/Behave/Blackboard/SSBehaviorTreeBlackboardProperty.h

    Include/SparkyStudios/AI/Behave/Core/SSBehaviorTreeFactory.h
    Include/SparkyStudios/AI/Behave/Core/SSBehaviorTreeNode.h
    Include/SparkyStudios/AI/Behave/Core/SSBehaviorTreeRegistry.h

    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterStringNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterStringNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionGetBlendInTimeNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionGetBlendOutTimeNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionGetLoopMotionNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionGetMotionNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionGetPlaySpeedNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionGetPlayTimeNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionPlayNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetBlendInTimeNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetLoopMotionNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetMirrorMotionNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetMotionNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetPlaySpeedNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetPlayTimeNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetRetargetMotionNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Animation/SimpleMotionSetReverseMotionNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Common/DebugMessageNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Common/WaitNode.h
    Include/SparkyStudios/AI/Behave/Nodes/Navigation/NavigationFindPathToEntityNode.h


    Source/Assets/SSBehaviorTreeAsset.h
    Source/Assets/SSBehaviorTreeAsset.cpp

    Source/Blackboard/SSBehaviorTreeBlackboard.cpp
    Source/Blackboard/SSBehaviorTreeBlackboardProperty.cpp

    Source/Core/SSBehaviorTreeFactory.cpp
    Source/Core/SSBehaviorTreeNode.cpp
    Source/Core/SSBehaviorTreeRegistry.cpp

    Source/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterStringNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterStringNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.cpp
    Source/Nodes/Animation/SimpleMotionGetBlendInTimeNode.cpp
    Source/Nodes/Animation/SimpleMotionGetBlendOutTimeNode.cpp
    Source/Nodes/Animation/SimpleMotionGetLoopMotionNode.cpp
    Source/Nodes/Animation/SimpleMotionGetMotionNode.cpp
    Source/Nodes/Animation/SimpleMotionGetPlaySpeedNode.cpp
    Source/Nodes/Animation/SimpleMotionGetPlayTimeNode.cpp
    Source/Nodes/Animation/SimpleMotionPlayNode.cpp
    Source/Nodes/Animation/SimpleMotionSetBlendInTimeNode.cpp
    Source/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.cpp
    Source/Nodes/Animation/SimpleMotionSetLoopMotionNode.cpp
    Source/Nodes/Animation/SimpleMotionSetMirrorMotionNode.cpp
    Source/Nodes/Animation/SimpleMotionSetMotionNode.cpp
    Source/Nodes/Animation/SimpleMotionSetPlaySpeedNode.cpp
    Source/Nodes/Animation/SimpleMotionSetPlayTimeNode.cpp
    Source/Nodes/Animation/SimpleMotionSetRetargetMotionNode.cpp
    Source/Nodes/Animation/SimpleMotionSetReverseMotionNode.cpp
    Source/Nodes/Common/DebugMessageNode.cpp
    Source/Nodes/Common/WaitNode.cpp
    Source/Nodes/Navigation/NavigationFindPathToEntityNode.cpp

    Source/SSBehaviorTreeComponent.h
    Source/SSBehaviorTreeComponent.cpp
    Source/SSBehaviorTreeModuleInterface.h
    Source/SSBehaviorTreeSystemComponent.cpp
    Source/SSBehaviorTreeSystemComponent.h

)
