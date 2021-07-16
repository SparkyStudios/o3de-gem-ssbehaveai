
set(FILES
    Source/StdAfx.cpp
    Source/StdAfx.h

    Include/SparkyStudios/AI/BehaviorTree/SSBehaviorTreeBus.h

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
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Common/DebugMessageNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Common/WaitNode.h
    Include/SparkyStudios/AI/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.h


    Source/Blackboard/SSBehaviorTreeBlackboardProperty.cpp

    Source/Core/SSBehaviorTreeFactory.cpp
    Source/Core/SSBehaviorTreeNode.cpp
    Source/Core/SSBehaviorTreeRegistry.cpp
    
    Source/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.cpp
    Source/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.cpp
    Source/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.cpp
    Source/Nodes/Common/DebugMessageNode.cpp
    Source/Nodes/Common/WaitNode.cpp
    Source/Nodes/Navigation/NavigationFindPathToEntityNode.cpp

    Source/SSBehaviorTreeModuleInterface.h
    Source/SSBehaviorTreeSystemComponent.cpp
    Source/SSBehaviorTreeSystemComponent.h
)
