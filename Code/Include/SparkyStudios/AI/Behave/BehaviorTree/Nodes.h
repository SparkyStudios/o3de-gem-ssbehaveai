// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <SparkyStudios/AI/Behave/BehaviorTree/Blackboard/Blackboard.h>

// Animation
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterBoolNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterFloatNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationEulerNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterRotationNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterStringNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector2Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphGetNamedParameterVector3Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterBoolNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterFloatNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationEulerNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterRotationNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterStringNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector2Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/AnimGraphSetNamedParameterVector3Node.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendInTimeNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetBlendOutTimeNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetLoopMotionNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetMotionNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetPlaySpeedNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionGetPlayTimeNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionPlayNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendInTimeNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetBlendOutTimeNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetLoopMotionNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetMirrorMotionNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetMotionNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetPlaySpeedNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetPlayTimeNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetRetargetMotionNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Animation/SimpleMotionSetReverseMotionNode.h>

// Common
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Common/DebugMessageNode.h>
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Common/WaitNode.h>

// Navigation
#include <SparkyStudios/AI/Behave/BehaviorTree/Nodes/Navigation/NavigationFindPathToEntityNode.h>

#ifndef __SS_BEHAVEAI_BEHAVIORTREE_NODES_REGISTERER__
#define __SS_BEHAVEAI_BEHAVIORTREE_NODES_REGISTERER__

namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes
{
    AZ_INLINE void RegisterDefaultNodes(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        // Navigation
        Navigation::NavigationFindPathToEntityNode::RegisterNode(registry);

        // Animation
        Animation::AnimGraphGetNamedParameterBoolNode::RegisterNode(registry);
        Animation::AnimGraphGetNamedParameterFloatNode::RegisterNode(registry);
        Animation::AnimGraphGetNamedParameterRotationEulerNode::RegisterNode(registry);
        Animation::AnimGraphGetNamedParameterRotationNode::RegisterNode(registry);
        Animation::AnimGraphGetNamedParameterStringNode::RegisterNode(registry);
        Animation::AnimGraphGetNamedParameterVector2Node::RegisterNode(registry);
        Animation::AnimGraphGetNamedParameterVector3Node::RegisterNode(registry);
        Animation::AnimGraphSetNamedParameterBoolNode::RegisterNode(registry);
        Animation::AnimGraphSetNamedParameterFloatNode::RegisterNode(registry);
        Animation::AnimGraphSetNamedParameterRotationEulerNode::RegisterNode(registry);
        Animation::AnimGraphSetNamedParameterRotationNode::RegisterNode(registry);
        Animation::AnimGraphSetNamedParameterStringNode::RegisterNode(registry);
        Animation::AnimGraphSetNamedParameterVector2Node::RegisterNode(registry);
        Animation::AnimGraphSetNamedParameterVector3Node::RegisterNode(registry);
        Animation::SimpleMotionGetBlendInTimeNode::RegisterNode(registry);
        Animation::SimpleMotionGetBlendOutTimeNode::RegisterNode(registry);
        Animation::SimpleMotionGetLoopMotionNode::RegisterNode(registry);
        Animation::SimpleMotionGetMotionNode::RegisterNode(registry);
        Animation::SimpleMotionGetPlaySpeedNode::RegisterNode(registry);
        Animation::SimpleMotionGetPlayTimeNode::RegisterNode(registry);
        Animation::SimpleMotionPlayNode::RegisterNode(registry);
        Animation::SimpleMotionSetBlendInTimeNode::RegisterNode(registry);
        Animation::SimpleMotionSetBlendOutTimeNode::RegisterNode(registry);
        Animation::SimpleMotionSetLoopMotionNode::RegisterNode(registry);
        Animation::SimpleMotionSetMirrorMotionNode::RegisterNode(registry);
        Animation::SimpleMotionSetMotionNode::RegisterNode(registry);
        Animation::SimpleMotionSetPlaySpeedNode::RegisterNode(registry);
        Animation::SimpleMotionSetPlayTimeNode::RegisterNode(registry);
        Animation::SimpleMotionSetRetargetMotionNode::RegisterNode(registry);
        Animation::SimpleMotionSetReverseMotionNode::RegisterNode(registry);

        // Common
        Common::DebugMessageNode::RegisterNode(registry);
        Common::WaitNode::RegisterNode(registry);
    }

    AZ_INLINE void RegisterDefaultProperties(const AZStd::shared_ptr<Core::Registry>& registry)
    {
        using namespace Blackboard;

        // Register properties
        registry->RegisterProperty<BlackboardPropertyBoolean>("bool");
        registry->RegisterProperty<BlackboardPropertyNumber>("float");
        registry->RegisterProperty<BlackboardPropertyNumber>("double");
        registry->RegisterProperty<BlackboardPropertyNumber>("long");
        registry->RegisterProperty<BlackboardPropertyNumber>("int");
        registry->RegisterProperty<BlackboardPropertyString>("AZStd::string");
        registry->RegisterProperty<BlackboardPropertyEntityRef>("AZ::EntityId");
        registry->RegisterProperty<BlackboardPropertyVector2>("AZ::Vector2");
    }
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Nodes
#endif
