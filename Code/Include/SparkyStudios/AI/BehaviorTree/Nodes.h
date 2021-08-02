#pragma once

#include <AzCore/Math/Vector2.h>
#include <AzCore/std/string/conversions.h>
#include <AzCore/std/string/string.h>

#include <SparkyStudios/AI/BehaviorTree/Blackboard/SSBehaviorTreeBlackboard.h>

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

#ifndef __SS_BEHAVIORTREE_NODES_REGISTERER__
#define __SS_BEHAVIORTREE_NODES_REGISTERER__

namespace SparkyStudios::AI::BehaviorTree::Nodes
{
    static void RegisterDefaultNodes(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // Navigation
        Nodes::Navigation::NavigationFindPathToEntityNode::RegisterNode(registry);

        // Animation
        Nodes::Animation::AnimGraphGetNamedParameterBoolNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphGetNamedParameterFloatNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphGetNamedParameterRotationEulerNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphGetNamedParameterRotationNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphGetNamedParameterStringNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphGetNamedParameterVector2Node::RegisterNode(registry);
        Nodes::Animation::AnimGraphGetNamedParameterVector3Node::RegisterNode(registry);
        Nodes::Animation::AnimGraphSetNamedParameterBoolNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphSetNamedParameterFloatNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphSetNamedParameterRotationEulerNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphSetNamedParameterRotationNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphSetNamedParameterStringNode::RegisterNode(registry);
        Nodes::Animation::AnimGraphSetNamedParameterVector2Node::RegisterNode(registry);
        Nodes::Animation::AnimGraphSetNamedParameterVector3Node::RegisterNode(registry);
        Nodes::Animation::SimpleMotionGetBlendInTimeNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionGetBlendOutTimeNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionGetLoopMotionNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionGetMotionNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionGetPlaySpeedNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionGetPlayTimeNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionPlayNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetBlendInTimeNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetBlendOutTimeNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetLoopMotionNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetMirrorMotionNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetMotionNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetPlaySpeedNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetPlayTimeNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetRetargetMotionNode::RegisterNode(registry);
        Nodes::Animation::SimpleMotionSetReverseMotionNode::RegisterNode(registry);

        // Common
        Nodes::Common::DebugMessageNode::RegisterNode(registry);
        Nodes::Common::WaitNode::RegisterNode(registry);
    }

    static void RegisterDefaultProperties(const AZStd::shared_ptr<Core::SSBehaviorTreeRegistry>& registry)
    {
        // Register properties
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyBoolean>("bool");
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyNumber>("float");
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyNumber>("double");
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyNumber>("long");
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyNumber>("int");
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyString>("AZStd::string");
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyEntityRef>("AZ::EntityId");
        registry->RegisterProperty<Blackboard::SSBehaviorTreeBlackboardPropertyVector2>("AZ::Vector2");
    }
} // namespace SparkyStudios::AI::BehaviorTree::Nodes
#endif