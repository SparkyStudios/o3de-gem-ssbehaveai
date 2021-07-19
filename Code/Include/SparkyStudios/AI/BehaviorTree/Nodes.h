#pragma once

#include <AzCore/Math/Vector2.h>
#include <AzCore/std/string/conversions.h>
#include <AzCore/std/string/string.h>

// Animation
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterStringNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterStringNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendInTimeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendOutTimeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetLoopMotionNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetMotionNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetPlaySpeedNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionGetPlayTimeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionPlayNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendInTimeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetLoopMotionNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetMirrorMotionNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetMotionNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetPlaySpeedNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetPlayTimeNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetRetargetMotionNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Animation/SimpleMotionSetReverseMotionNode.h>

// Common
#include <SparkyStudios/AI/BehaviorTree/Nodes/Common/DebugMessageNode.h>
#include <SparkyStudios/AI/BehaviorTree/Nodes/Common/WaitNode.h>

// Navigation
#include <SparkyStudios/AI/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.h>
