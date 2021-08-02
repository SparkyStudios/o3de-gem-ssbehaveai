#include <Editor/Application/precompiled.h>

#include <Editor/Core/EditorBase.h>

#include <QDebug>

#include <AzCore/std/function/function_template.h>

#include <SparkyStudios/AI/BehaviorTree/Core/SSBehaviorTreeNode.h>
#include <SparkyStudios/AI/BehaviorTree/SSBehaviorTreeBus.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Core
{
    PortModel& PortModel::operator=(const BT::PortInfo& src)
    {
        this->direction = src.direction();
        this->description = QString::fromStdString(src.description());
        this->typeName = QString::fromStdString(BT::demangle(src.type()));
        this->defaultValue = QString::fromStdString(src.defaultValue());

        return *this;
    }

    bool NodeModel::operator==(const NodeModel& other) const
    {
        bool is_same = (type == other.type && ports.size() == other.ports.size() && registrationId == other.registrationId);
        if (!is_same)
            return false;

        auto other_it = other.ports.begin();
        for (const auto& port_it : ports)
        {
            if (port_it.first != other_it->first)
            {
                return false;
            }
            if (port_it.second.direction != other_it->second.direction)
            {
                return false;
            }
            if (port_it.second.typeName != other_it->second.typeName)
            {
                return false;
            }
            other_it++;
        }
        return true;
    }

    NodeModel& NodeModel::operator=(const BT::TreeNodeManifest& src)
    {
        this->type = src.type;
        this->registrationId = QString::fromStdString(src.registration_ID);

        for (const auto& port_it : src.ports)
        {
            const auto& port_name = port_it.first;
            const auto& bt_port = port_it.second;
            PortModel port_model;
            port_model = bt_port;
            this->ports.insert({ QString::fromStdString(port_name), std::move(port_model) });
        }

        return *this;
    }

    bool BlackboardPropertyModel::operator==(const BlackboardPropertyModel& other) const
    {
        return (name == other.name) && (suffix == other.suffix) && (type == other.type) && (description == other.description) &&
            (isPrivate == other.isPrivate);
    }

    GraphicMode GetGraphicModeFromString(const QString& str)
    {
        if (str == "EDITOR")
            return GraphicMode::EDITOR;

        return GraphicMode::MONITOR;
    }

    const char* ToString(GraphicMode mode)
    {
        if (mode == GraphicMode::EDITOR)
            return "EDITOR";
        if (mode == GraphicMode::MONITOR)
            return "MONITOR";

        return nullptr;
    }

    const NodeModels& BuiltInNodeModels(const BehaviorTree::Core::SSBehaviorTreeFactory& factory)
    {
        static NodeModels builtin_node_models = [&factory]() -> NodeModels
        {
            const auto& btFactory = factory.GetRegistry()->GetBTFactory();

            btFactory->registerNodeType<BT::SubtreeNode>("Root");

            const std::set<std::string>& builtinNodes = btFactory->builtinNodes();

            NodeModels out;
            for (const auto& it : btFactory->manifests())
            {
                const auto& model_name = it.first;
                if (model_name == "SubTree" || model_name == "SubTreePlus")
                    continue;

                const auto& bt_model = it.second;
                NodeModel ed_model;
                ed_model = bt_model;

                if (builtinNodes.count(model_name) == 0)
                {
                    const auto* node = btFactory->instantiateTreeNode(model_name, model_name, {}).release();

                    if (const auto* ss_node = dynamic_cast<const BehaviorTree::Core::SSBehaviorTreeNode*>(node))
                    {
                        ed_model.category = QString::fromStdString(ss_node->NodeCategory());
                    }

                    delete node;
                }
                else
                {
                    ed_model.category = "Builtin";
                }

                out.insert({ QString::fromStdString(model_name), std::move(ed_model) });
            }

            return out;
        }();

        return builtin_node_models;
    }

    bool AbstractBehaviorTreeNode::operator==(const AbstractBehaviorTreeNode& other) const
    {
        bool same_registration = model.registrationId == other.model.registrationId;
        return same_registration && status == other.status && size == other.size &&
            // position == other.position &&
            instanceName == other.instanceName;
    }

    AbstractBehaviorTree::~AbstractBehaviorTree()
    {
        Clear();
    }

    const AbstractBehaviorTreeNode* AbstractBehaviorTree::RootNode() const
    {
        if (_nodes.empty())
            return nullptr;
        return &_nodes.front();
    }

    AbstractBehaviorTreeNode* AbstractBehaviorTree::RootNode()
    {
        if (_nodes.empty())
            return nullptr;
        return &_nodes.front();
    }

    std::vector<const AbstractBehaviorTreeNode*> AbstractBehaviorTree::FindNodes(const QString& instanceName)
    {
        std::vector<const AbstractBehaviorTreeNode*> out;
        out.reserve(4);

        for (const auto& node : _nodes)
            if (node.instanceName == instanceName)
                out.push_back(&node);

        return out;
    }

    const AbstractBehaviorTreeNode* AbstractBehaviorTree::FindFirstNode(const QString& instanceName)
    {
        for (const auto& node : _nodes)
            if (node.instanceName == instanceName)
                return (&node);

        return nullptr;
    }

    AbstractBehaviorTreeNode* AbstractBehaviorTree::AddNode(AbstractBehaviorTreeNode* parent, AbstractBehaviorTreeNode&& newNode)
    {
        size_t index = _nodes.size();
        newNode.index = index;

        if (parent)
        {
            _nodes.push_back(std::move(newNode));
            parent->childrenIndex.push_back(index);
        }
        else
        {
            _nodes.clear();
            _nodes.push_back(newNode);
        }

        return &_nodes.back();
    }

    void AbstractBehaviorTree::DebugPrint() const
    {
        if (!RootNode())
        {
            qDebug() << "Empty BehaviorTree";
            return;
        }

        std::function<void(const AbstractBehaviorTreeNode*, int)> recursiveStep;
        recursiveStep = [&](const AbstractBehaviorTreeNode* node, int indent)
        {
            for (int i = 0; i < indent; i++)
                AZ_Printf("SSBehaviorTreeEditor", "    ");

            AZ_Printf(
                "SSBehaviorTreeEditor", "%s (%s)", node->instanceName.toStdString().c_str(),
                node->model.registrationId.toStdString().c_str());

            for (size_t index : node->childrenIndex)
            {
                auto childNode = &_nodes[index];
                recursiveStep(childNode, indent + 1);
            }
        };

        recursiveStep(RootNode(), 0);
    }

    void AbstractBehaviorTree::Clear()
    {
        _nodes.resize(0);
    }

    bool AbstractBehaviorTree::operator==(const AbstractBehaviorTree& other) const
    {
        if (_nodes.size() != other._nodes.size())
            return false;

        for (size_t index = 0; index < _nodes.size(); index++)
            if (_nodes[index] != other._nodes[index])
                return false;

        return true;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core
