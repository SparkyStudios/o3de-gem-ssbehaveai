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

#include <Editor/Application/precompiled.h>

#include <QPointF>
#include <QString>

#include <AzCore/AzCoreModule.h>
#include <AzCore/std/containers/deque.h>
#include <AzCore/std/containers/map.h>
#include <AzCore/std/containers/set.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/containers/vector.h>

#include <SparkyStudios/AI/Behave/BehaviorTree/Core/Factory.h>

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core
{
    using BT::NodeStatus;
    using BT::NodeType;
    using BT::PortDirection;

    typedef std::map<QString, QString> PortsMapping;

    // Alternative type similar to BT::PortInfo
    struct PortModel
    {
        PortModel()
            : direction(PortDirection::INOUT)
        {
        }

        QString typeName;
        PortDirection direction;
        QString description;
        QString defaultValue;

        PortModel& operator=(const BT::PortInfo& src);
    };

    typedef std::map<QString, PortModel> PortModels;

    struct NodeModel
    {
        NodeType type;
        QString registrationId;
        PortModels ports;
        QString category;

        bool operator==(const NodeModel& other) const;
        bool operator!=(const NodeModel& other) const
        {
            return !(*this == other);
        }

        NodeModel& operator=(const BT::TreeNodeManifest& src);
    };

    typedef std::map<QString, NodeModel> NodeModels;

    struct BlackboardPropertyModel
    {
        QString mName;
        QString mDescription;
        QString mSuffix;
        QString mType;
        float mOrder = FLT_MAX;
        bool mIsPrivate = false;

        bool operator==(const BlackboardPropertyModel& other) const;
        bool operator!=(const BlackboardPropertyModel& other) const
        {
            return !(*this == other);
        }
    };

    typedef std::map<QString, BlackboardPropertyModel> BlackboardPropertyModels;

    enum class GraphicMode
    {
        EDITOR,
        MONITOR
    };

    /// <summary>
    /// Gets the GraphicMode from the given <paramref name="str" />.
    /// </summary>
    /// <param name="str">The string to convert into GraphicMode.</param>
    /// <returns>A GraphicMode.</returns>
    GraphicMode GetGraphicModeFromString(const QString& str);

    /// <summary>
    /// Gets the string value of the given <paramref name="mode" />.
    /// </summary>
    /// <param name="mode">The GraphicMode to convert into string.</param>
    /// <returns>The string representation of the GraphicMode.</returns>
    const char* ToString(GraphicMode mode);

    /// <summary>
    /// Gets the list of built-in models.
    /// </summary>
    const NodeModels& BuiltInNodeModels(const BehaviorTree::Core::Factory& factory);

    struct AbstractBehaviorTreeNode
    {
        AbstractBehaviorTreeNode()
            : index(0)
            , status(NodeStatus::IDLE)
            , graphicNode(nullptr)
        {
            model.type = NodeType::UNDEFINED;
        }

        NodeModel model;
        PortsMapping portsMapping;
        size_t index;
        QString instanceName;
        NodeStatus status;
        QSizeF size;
        QPointF position;
        std::vector<size_t> childrenIndex;
        QtNodes::Node* graphicNode;

        bool operator==(const AbstractBehaviorTreeNode& other) const;
        bool operator!=(const AbstractBehaviorTreeNode& other) const
        {
            return !(*this == other);
        }
    };

    class AbstractBehaviorTree
    {
    public:
        AZ_CLASS_ALLOCATOR(AbstractBehaviorTree, AZ::SystemAllocator, 0);
        AZ_RTTI(AbstractBehaviorTree, "{22aa8f01-276b-402e-9e92-bc857982e6f4}");

        typedef std::deque<AbstractBehaviorTreeNode> NodesVector;

        AbstractBehaviorTree() = default;

        virtual ~AbstractBehaviorTree();

        size_t NodesCount() const
        {
            return _nodes.size();
        }

        const NodesVector& Nodes() const
        {
            return _nodes;
        }

        NodesVector& Nodes()
        {
            return _nodes;
        }

        const AbstractBehaviorTreeNode* Node(size_t index) const
        {
            return &_nodes.at(index);
        }

        AbstractBehaviorTreeNode* Node(size_t index)
        {
            return &_nodes.at(index);
        }

        const AbstractBehaviorTreeNode* RootNode() const;
        AbstractBehaviorTreeNode* RootNode();

        std::vector<const AbstractBehaviorTreeNode*> FindNodes(const QString& instanceName);
        const AbstractBehaviorTreeNode* FindFirstNode(const QString& instanceName);

        AbstractBehaviorTreeNode* AddNode(AbstractBehaviorTreeNode* parent, AbstractBehaviorTreeNode&& newNode);

        void DebugPrint() const;

        /// <summary>
        /// Clears the current behavior tree. Deletes all nodes.
        /// </summary>
        void Clear();

        bool operator==(const AbstractBehaviorTree& other) const;
        bool operator!=(const AbstractBehaviorTree& other) const
        {
            return !(*this == other);
        }

    private:
        NodesVector _nodes;
    };

    Q_DECLARE_METATYPE(AbstractBehaviorTree);
} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core
