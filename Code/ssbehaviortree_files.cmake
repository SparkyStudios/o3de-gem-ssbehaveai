
set(FILES
    Source/StdAfx.cpp
    Source/StdAfx.h

    Include/SparkyStudios/AI/BehaviorTree/Nodes.h
    Include/SparkyStudios/AI/BehaviorTree/SSBehaviorTreeBus.h

    Include/SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h
    Include/SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboardProperty.h

    Include/SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeFactory.h
    Include/SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h
    Include/SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeRegistry.h

    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterStringNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterStringNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendInTimeNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendOutTimeNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetLoopMotionNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetMotionNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetPlaySpeedNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetPlayTimeNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionPlayNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendInTimeNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetLoopMotionNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetMirrorMotionNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetMotionNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetPlaySpeedNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetPlayTimeNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetRetargetMotionNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetReverseMotionNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Common/DebugMessageNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Common/WaitNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.h


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
