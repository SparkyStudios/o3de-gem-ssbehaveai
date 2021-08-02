#include <Editor/Application/precompiled.h>

#include <Editor/Core/EditorUtils.h>

#include <QDebug>
#include <QDomDocument>
#include <QMessageBox>

#include <AzCore/std/function/function_template.h>

#include <Editor/Core/Models/SubtreeNodeModel.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Core
{
    using QtNodes::DataModelRegistry;
    using QtNodes::FlowScene;
    using QtNodes::Node;
    using QtNodes::PortLayout;
    using QtNodes::PortType;

    QtNodes::Node* findRoot(const QtNodes::FlowScene& scene)
    {
        Node* root = nullptr;

        for (auto& it : scene.nodes())
        {
            Node* node = it.second.get();
            if (node->nodeDataModel()->nPorts(PortType::In) == 0)
            {
                if (!root)
                    root = node;
                else
                    return nullptr;
            }
            else if (node->nodeState().connections(PortType::In, 0).empty())
            {
                if (!root)
                    root = node;
                else
                    return nullptr;
            }
        }

        return root;
    }

    std::vector<Node*> getChildren(const QtNodes::FlowScene& scene, const Node& parent_node, bool ordered)
    {
        std::vector<Node*> children;

        if (parent_node.nodeDataModel()->nPorts(PortType::Out) == 0)
        {
            return children;
        }

        const auto& conn_out = parent_node.nodeState().connections(PortType::Out, 0);
        children.reserve(conn_out.size());

        for (auto& it : conn_out)
        {
            auto child_node = it.second->getNode(PortType::In);
            if (child_node)
            {
                children.push_back(child_node);
            }
        }

        if (ordered && children.size() > 1)
        {
            if (scene.layout() == PortLayout::Vertical)
            {
                std::sort(
                    children.begin(), children.end(),
                    [&](const Node* a, const Node* b)
                    {
                        double pos_a = scene.getNodePosition(*a).x() + scene.getNodeSize(*a).width() * 0.5;
                        double pos_b = scene.getNodePosition(*b).x() + scene.getNodeSize(*b).width() * 0.5;
                        return pos_a < pos_b;
                    });
            }
            else
            {
                std::sort(
                    children.begin(), children.end(),
                    [&](const Node* a, const Node* b)
                    {
                        double pos_a = scene.getNodePosition(*a).y() + scene.getNodeSize(*a).height() * 0.5;
                        double pos_b = scene.getNodePosition(*b).y() + scene.getNodeSize(*b).height() * 0.5;
                        return pos_a < pos_b;
                    });
            }
        }

        return children;
    }

    //---------------------------------------------------

    void RecursiveNodeReorder(AbstractBehaviorTree& tree, PortLayout layout)
    {
        std::function<void(unsigned, AbstractBehaviorTreeNode*)> recursiveStep;

        std::vector<QPointF> layer_cursor;
        std::vector<std::vector<AbstractBehaviorTreeNode*>> nodes_by_level(1);

        const qreal LEVEL_SPACING = 80;
        const qreal NODE_SPACING = 40;

        recursiveStep = [&](unsigned current_layer, AbstractBehaviorTreeNode* node)
        {
            node->position = layer_cursor[current_layer];
            nodes_by_level[current_layer].push_back(node);

            //------------------------------------
            if (node->childrenIndex.size() == 0)
            {
                return;
            }
            qreal recommended_pos = NODE_SPACING * 0.5;

            current_layer++;

            if (layout == PortLayout::Vertical)
            {
                recommended_pos += (node->position.x() + node->size.width() * 0.5);
                for (size_t index : node->childrenIndex)
                {
                    const auto& child_node = tree.Nodes()[index];
                    recommended_pos -= (child_node.size.width() + NODE_SPACING) * 0.5;
                }

                if (current_layer >= layer_cursor.size())
                {
                    QPointF new_cursor(recommended_pos, node->position.y() + LEVEL_SPACING);
                    layer_cursor.push_back(new_cursor);
                    nodes_by_level.push_back(std::vector<AbstractBehaviorTreeNode*>());
                }
                else
                {
                    recommended_pos = std::max(recommended_pos, layer_cursor[current_layer].x());
                    layer_cursor[current_layer].setX(recommended_pos);
                }
            } //------------------------------------
            else
            {
                recommended_pos += node->position.y() + node->size.height() * 0.5;
                for (size_t index : node->childrenIndex)
                {
                    const auto& child_node = tree.Nodes()[index];
                    recommended_pos -= (child_node.size.height() + NODE_SPACING) * 0.5;
                }

                if (current_layer >= layer_cursor.size())
                {
                    QPointF new_cursor(node->position.x() + LEVEL_SPACING, recommended_pos);
                    layer_cursor.push_back(new_cursor);
                    nodes_by_level.push_back(std::vector<AbstractBehaviorTreeNode*>());
                }
                else
                {
                    recommended_pos = std::max(recommended_pos, layer_cursor[current_layer].y());
                    layer_cursor[current_layer].setY(recommended_pos);
                }
            }
            //------------------------------------

            auto initial_pos = layer_cursor[current_layer];

            for (size_t index : node->childrenIndex)
            {
                AbstractBehaviorTreeNode* child_node = tree.Node(index);

                recursiveStep(current_layer, child_node);
                if (layout == PortLayout::Vertical)
                {
                    layer_cursor[current_layer] += QPointF(child_node->size.width() + NODE_SPACING, 0);
                }
                else
                {
                    layer_cursor[current_layer] += QPointF(0, child_node->size.height() + NODE_SPACING);
                }
            }

            auto final_pos = layer_cursor[current_layer];

            if (node->childrenIndex.size() > 0)
            {
                auto first_child = tree.Node(node->childrenIndex.front());
                auto last_child = tree.Node(node->childrenIndex.back());

                initial_pos = QPointF(
                    first_child->position.x() + first_child->size.width() * 0.5,
                    first_child->position.y() + first_child->size.height() * 0.5);

                final_pos = QPointF(
                    last_child->position.x() + last_child->size.width() * 0.5, last_child->position.y() + last_child->size.height() * 0.5);
            }

            // rebalance father
            QPointF pos_offset(0, 0);
            if (layout == PortLayout::Vertical)
            {
                double new_x = (final_pos.x() + initial_pos.x()) * 0.5 - node->size.width() * 0.5;
                double diff = node->position.x() - new_x;
                pos_offset = QPointF(-diff, 0);
            }
            else
            {
                double new_y = (final_pos.y() + initial_pos.y()) * 0.5 - node->size.height() * 0.5;
                double diff = node->position.y() - new_y;
                pos_offset = QPointF(0, -diff);
            }

            node->position += pos_offset;
            layer_cursor[current_layer - 1] += pos_offset;
        };

        auto root_node = tree.RootNode();
        layer_cursor.push_back(QPointF(-root_node->size.width() * 0.5, -root_node->size.height() * 0.5));
        recursiveStep(0, root_node);

        if (layout == PortLayout::Vertical)
        {
            qreal offset = root_node->size.height() + LEVEL_SPACING;
            for (unsigned i = 1; i < nodes_by_level.size(); i++)
            {
                qreal max_height = 0;
                for (auto node : nodes_by_level[i])
                {
                    max_height = std::max(max_height, node->size.height());
                    node->position.setY(offset);
                }
                offset += max_height + LEVEL_SPACING;
            }
        }
        else
        {
            qreal offset = root_node->size.width() + LEVEL_SPACING;
            for (unsigned i = 1; i < nodes_by_level.size(); i++)
            {
                qreal max_width = 0;
                for (auto node : nodes_by_level[i])
                {
                    max_width = std::max(max_width, node->size.width());
                    node->position.setX(offset);
                }
                offset += max_width + LEVEL_SPACING;
            }
        }
    }

    void NodeReorder(QtNodes::FlowScene& scene, AbstractBehaviorTree& tree)
    {
        for (const auto& abs_node : tree.Nodes())
        {
            Node* node = abs_node.graphicNode;
            if (node == nullptr)
            {
                AZ_Error("SSBehaviorTree", false, "one or more nodes haven't been created yet");
                return;
            }
        }

        if (tree.NodesCount() == 0)
        {
            return;
        }

        RecursiveNodeReorder(tree, scene.layout());

        for (const auto& abs_node : tree.Nodes())
        {
            Node* node = abs_node.graphicNode;
            scene.setNodePosition(*node, abs_node.position);
        }
    }

    AbstractBehaviorTree BuildTreeFromScene(const QtNodes::FlowScene* scene, QtNodes::Node* root_node)
    {
        if (!root_node)
        {
            root_node = findRoot(*scene);
        }

        if (!root_node)
        {
            if (scene->nodes().size() != 0)
            {
                qDebug() << "Error: can not create a tree from a scene unless it has a single root node ";
            }
            return AbstractBehaviorTree();
        }

        AbstractBehaviorTree tree;

        std::function<void(AbstractBehaviorTreeNode*, QtNodes::Node*)> pushRecursively;

        pushRecursively = [&](AbstractBehaviorTreeNode* parent, QtNodes::Node* node)
        {
            AbstractBehaviorTreeNode abs_node;

            auto bt_model = static_cast<Models::SSBehaviorTreeNodeDataModel*>(node->nodeDataModel());

            abs_node.model = bt_model->model();
            abs_node.instanceName = bt_model->instanceName();
            abs_node.position = scene->getNodePosition(*node);
            abs_node.size = scene->getNodeSize(*node);
            abs_node.graphicNode = node;
            abs_node.portsMapping = bt_model->getCurrentPortMapping();

            auto added_node = tree.AddNode(parent, std::move(abs_node));

            auto children = getChildren(*scene, *node, true);

            for (auto& child_node : children)
            {
                pushRecursively(added_node, child_node);
            }
        };

        pushRecursively(nullptr, root_node);

        return tree;
    }

    AbstractBehaviorTree BuildTreeFromXML(const QDomElement& bt_root, const NodeModels& models)
    {
        AbstractBehaviorTree tree;

        if (bt_root.tagName() != "BehaviorTree")
        {
            AZ_Error("SSBehaviorTreeEditor", false, "Expecting a node called <BehaviorTree>");
            return tree;
        }

        //-------------------------------------
        std::function<void(AbstractBehaviorTreeNode*, QDomElement)> recursiveStep;
        recursiveStep = [&](AbstractBehaviorTreeNode* parent, QDomElement xml_node)
        {
            // The nodes with a ID used that QString to insert into the registry()
            QString modelID = xml_node.tagName();
            if (xml_node.hasAttribute("ID"))
            {
                modelID = xml_node.attribute("ID");
            }

            AbstractBehaviorTreeNode tree_node;

            auto model_it = models.find(modelID);
            if (model_it == models.end())
            {
                AZ_Error("SSBehaviorTree", false, (QString("This model has not been registered: ") + modelID).toStdString().c_str());
                return;
            }
            tree_node.model = model_it->second;

            if (xml_node.hasAttribute("name"))
            {
                tree_node.instanceName = (xml_node.attribute("name"));
            }
            else
            {
                tree_node.instanceName = modelID;
            }

            auto attributes = xml_node.attributes();
            for (int attr = 0; attr < attributes.size(); attr++)
            {
                auto attribute = attributes.item(attr).toAttr();
                if (attribute.name() != "ID" && attribute.name() != "name")
                {
                    tree_node.portsMapping.insert({ attribute.name(), attribute.value() });
                }
            }

            auto added_node = tree.AddNode(parent, std::move(tree_node));

            for (QDomElement child = xml_node.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
            {
                recursiveStep(added_node, child);
            }
        };

        auto first_child = bt_root.firstChildElement();
        if (first_child.tagName() == "Root")
        {
            QMessageBox::question(nullptr, "Fix your file!", "Please remove the node <Root> from your <BehaviorTree>", QMessageBox::Ok);
            first_child = first_child.firstChildElement();
        }

        // start recursion
        recursiveStep(nullptr, first_child);

        return tree;
    }

    std::pair<AbstractBehaviorTree, std::unordered_map<int, int>> BuildTreeFromFlatbuffers(
        const Serialization::BehaviorTree* fb_behavior_tree)
    {
        AbstractBehaviorTree tree;
        std::unordered_map<int, int> uid_to_index;

        AbstractBehaviorTreeNode abs_root;
        abs_root.instanceName = "Root";
        abs_root.model.registrationId = "Root";
        abs_root.model.registrationId = "Root";
        abs_root.childrenIndex.push_back(1);

        tree.AddNode(nullptr, std::move(abs_root));

        //-----------------------------------------
        NodeModels models;

        for (const Serialization::NodeModel* model_node : *(fb_behavior_tree->node_models()))
        {
            NodeModel model;
            model.registrationId = model_node->registration_name()->c_str();
            model.type = convert(model_node->type());

            for (const Serialization::PortModel* port : *(model_node->ports()))
            {
                PortModel port_model;
                QString port_name = port->port_name()->c_str();
                port_model.direction = convert(port->direction());
                port_model.typeName = port->type_info()->c_str();
                port_model.description = port->description()->c_str();

                model.ports.insert({ port_name, std::move(port_model) });
            }

            models.insert({ model.registrationId, std::move(model) });
        }

        //-----------------------------------------
        for (const Serialization::TreeNode* fb_node : *(fb_behavior_tree->nodes()))
        {
            AbstractBehaviorTreeNode abs_node;
            abs_node.instanceName = fb_node->instance_name()->c_str();
            const char* registration_ID = fb_node->registration_name()->c_str();
            abs_node.status = convert(fb_node->status());
            abs_node.model = (models[registration_ID]);

            for (const Serialization::PortConfig* pair : *(fb_node->port_remaps()))
            {
                abs_node.portsMapping.insert({ QString(pair->port_name()->c_str()), QString(pair->remap()->c_str()) });
            }
            size_t index = tree.NodesCount();
            abs_node.index = index;
            tree.Nodes().push_back(std::move(abs_node));
            uid_to_index.insert({ fb_node->uid(), int(index) });
        }

        for (size_t index = 0; index < fb_behavior_tree->nodes()->size(); index++)
        {
            const Serialization::TreeNode* fb_node = fb_behavior_tree->nodes()->Get(uint32_t(index));
            AbstractBehaviorTreeNode* abs_node = tree.Node(index + 1);
            for (const auto child_uid : *(fb_node->children_uid()))
            {
                int child_index = uid_to_index[child_uid];
                abs_node->childrenIndex.push_back(child_index);
            }
        }

        return { tree, uid_to_index };
    }

    std::pair<QtNodes::NodeStyle, QtNodes::ConnectionStyle> getStyleFromStatus(NodeStatus status, NodeStatus prev_status)
    {
        QtNodes::NodeStyle node_style;
        QtNodes::ConnectionStyle conn_style;

        float penWidth = 3.0;

        // TODO: should change connection color here
        // conn_style.HoveredColor = Qt::transparent;

        // printf("status=%d, old=%d\n", status, prev_status);

        if (status == NodeStatus::IDLE)
        {
            if (prev_status != NodeStatus::IDLE)
            {
                node_style.PenWidth *= penWidth;
                node_style.HoveredPenWidth = node_style.PenWidth;

                if (prev_status == NodeStatus::SUCCESS)
                {
                    node_style.NormalBoundaryColor = node_style.ShadowColor = QColor(100, 150, 100);
                    // conn_style.NormalColor = node_style.NormalBoundaryColor;
                }
                else if (prev_status == NodeStatus::RUNNING)
                {
                    node_style.NormalBoundaryColor = node_style.ShadowColor = QColor(150, 130, 40);
                    // conn_style.NormalColor = node_style.NormalBoundaryColor;
                }
                else if (prev_status == NodeStatus::FAILURE)
                {
                    node_style.NormalBoundaryColor = node_style.ShadowColor = QColor(150, 80, 80);
                    // conn_style.NormalColor = node_style.NormalBoundaryColor;
                }
            }

            return { node_style, conn_style };
        }

        node_style.PenWidth *= penWidth;
        node_style.HoveredPenWidth = node_style.PenWidth;

        if (status == NodeStatus::SUCCESS)
        {
            node_style.NormalBoundaryColor = node_style.ShadowColor = QColor(51, 200, 51);
            node_style.ShadowColor = QColor(51, 250, 51);
            // conn_style.NormalColor = node_style.NormalBoundaryColor;
        }
        else if (status == NodeStatus::RUNNING)
        {
            node_style.NormalBoundaryColor = node_style.ShadowColor = QColor(220, 140, 20);
            // conn_style.NormalColor = node_style.NormalBoundaryColor;
        }
        else if (status == NodeStatus::FAILURE)
        {
            node_style.NormalBoundaryColor = node_style.ShadowColor = QColor(250, 50, 50);
            // conn_style.NormalColor = node_style.NormalBoundaryColor;
        }

        return { node_style, conn_style };
    }

    QtNodes::Node* GetParentNode(QtNodes::Node* node)
    {
        using namespace QtNodes;
        auto conn_in = node->nodeState().connections(PortType::In, 0);
        if (conn_in.size() == 0)
        {
            return nullptr;
        }
        else
        {
            return conn_in.begin()->second->getNode(PortType::Out);
        }
    }

    std::set<QString> GetModelsToRemove(
        QWidget* parent, NodeModels& prev_models, const NodeModels& new_models, const BehaviorTree::Core::SSBehaviorTreeFactory& factory)
    {
        std::set<QString> prev_custom_models;
        const auto& models = BuiltInNodeModels(factory);

        if (prev_models.size() > models.size())
        {
            for (const auto& it : prev_models)
            {
                const QString& model_name = it.first;
                if (models.count(model_name) == 0 && new_models.count(model_name) == 0)
                {
                    prev_custom_models.insert(model_name);
                }
            }
        }

        if (prev_custom_models.size() > 0)
        {
            int ret = QMessageBox::question(
                parent, "Clear Palette?", "Do you want to remove the previously loaded custom nodes?", QMessageBox::No | QMessageBox::Yes);
            if (ret == QMessageBox::No)
            {
                prev_custom_models.clear();
            }
        }

        return prev_custom_models;
    }

    BT::NodeType convert(Serialization::NodeType type)
    {
        switch (type)
        {
        case Serialization::NodeType::ACTION:
            return BT::NodeType::ACTION;
        case Serialization::NodeType::DECORATOR:
            return BT::NodeType::DECORATOR;
        case Serialization::NodeType::CONTROL:
            return BT::NodeType::CONTROL;
        case Serialization::NodeType::CONDITION:
            return BT::NodeType::CONDITION;
        case Serialization::NodeType::SUBTREE:
            return BT::NodeType::SUBTREE;
        case Serialization::NodeType::UNDEFINED:
            return BT::NodeType::UNDEFINED;
        }
        return BT::NodeType::UNDEFINED;
    }

    BT::NodeStatus convert(Serialization::NodeStatus type)
    {
        switch (type)
        {
        case Serialization::NodeStatus::IDLE:
            return BT::NodeStatus::IDLE;
        case Serialization::NodeStatus::SUCCESS:
            return BT::NodeStatus::SUCCESS;
        case Serialization::NodeStatus::RUNNING:
            return BT::NodeStatus::RUNNING;
        case Serialization::NodeStatus::FAILURE:
            return BT::NodeStatus::FAILURE;
        }

        return BT::NodeStatus::IDLE;
    }

    BT::PortDirection convert(Serialization::PortDirection direction)
    {
        switch (direction)
        {
        case Serialization::PortDirection::INPUT:
            return BT::PortDirection::INPUT;
        case Serialization::PortDirection::OUTPUT:
            return BT::PortDirection::OUTPUT;
        case Serialization::PortDirection::INOUT:
            return BT::PortDirection::INOUT;
        }

        return BT::PortDirection::INOUT;
    }

    NodeModel buildTreeNodeModelFromXML(const QDomElement& node)
    {
        PortModels ports_list;

        QString tag_name = node.tagName();
        QString ID = tag_name;
        if (node.hasAttribute("ID"))
        {
            ID = QString(node.attribute("ID"));
        }

        const auto node_type = BT::convertFromString<BT::NodeType>(tag_name.toStdString());

        if (node_type == BT::NodeType::UNDEFINED)
        {
            return {};
        }

        // this make sense for ports inside the <BehaviorTree> tag
        QDomNamedNodeMap attributes = node.attributes();
        for (int i = 0; i < attributes.size(); i++)
        {
            QDomAttr attr = attributes.item(i).toAttr();

            QString attr_name(attr.name());
            if (attr_name != "ID" && attr_name != "name")
            {
                PortModel port_model;
                port_model.direction = PortDirection::INOUT;
                ports_list.insert({ attr_name, std::move(port_model) });
            }
        }

        // this is used for ports inside the <TreeNodesModel> tag
        const std::vector<std::pair<QString, PortDirection>> portsTypes = { { "input_port", PortDirection::INPUT },
                                                                            { "output_port", PortDirection::OUTPUT },
                                                                            { "inout_port", PortDirection::INOUT } };

        for (const auto& it : portsTypes)
        {
            for (QDomElement port_element = node.firstChildElement(it.first); !port_element.isNull();
                 port_element = port_element.nextSiblingElement(it.first))
            {
                PortModel port_model;
                port_model.direction = it.second;
                port_model.description = port_element.text();

                if (port_element.hasAttribute("type"))
                {
                    port_model.typeName = port_element.attribute("type");
                }
                if (port_element.hasAttribute("default"))
                {
                    port_model.defaultValue = port_element.attribute("default");
                }

                if (port_element.hasAttribute("name"))
                {
                    auto attr_name = port_element.attribute("name");
                    ports_list.insert({ attr_name, std::move(port_model) });
                }
            }
        }

        return { node_type, ID, ports_list };
    }

    //------------------------------------------------------------------

    NodeModels ReadTreeNodesModel(const QDomElement& root)
    {
        NodeModels models;
        using QtNodes::DataModelRegistry;

        auto model_root = root.firstChildElement("TreeNodesModel");

        if (!model_root.isNull())
        {
            for (QDomElement node = model_root.firstChildElement(); !node.isNull(); node = node.nextSiblingElement())
            {
                auto model = buildTreeNodeModelFromXML(node);
                models.insert({ model.registrationId, model });
            }
        }

        std::function<void(QDomElement)> recursiveStep;
        recursiveStep = [&](QDomElement node)
        {
            auto model = buildTreeNodeModelFromXML(node);
            if (model.type != NodeType::UNDEFINED && model.registrationId.isEmpty() == false && models.count(model.registrationId) == 0)
            {
                models.insert({ model.registrationId, model });
            }

            for (QDomElement child = node.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
            {
                recursiveStep(child);
            }
        };

        for (QDomElement bt_root = root.firstChildElement("BehaviorTree"); !bt_root.isNull();
             bt_root = bt_root.nextSiblingElement("BehaviorTree"))
        {
            auto first_child = bt_root.firstChildElement();
            if (first_child.tagName() == "Root")
            {
                first_child = first_child.firstChildElement();
            }
            recursiveStep(bt_root.firstChildElement());
        }

        return models;
    }

    BlackboardPropertyModels ReadBlackboardPropertiesModel(const QDomElement& root)
    {
        BlackboardPropertyModels properties;

        auto blackboard_root = root.firstChildElement("Blackboard");

        if (!blackboard_root.isNull())
        {
            for (QDomElement node = blackboard_root.firstChildElement(); !node.isNull(); node = node.nextSiblingElement())
            {
                BlackboardPropertyModel model;
                model.name = node.attribute("name");
                model.description = node.attribute("description");
                model.type = node.attribute("type");
                model.suffix = node.attribute("suffix");
                model.isPrivate = node.attribute("private").isNull() ? false : node.attribute("private") == "true";
                properties.insert({ model.name, model });
            }
        }

        return properties;
    }

    void RecursivelyCreateXml(
        const FlowScene& scene,
        QDomDocument& doc,
        QDomElement& parent_element,
        const Node* node,
        const BehaviorTree::Core::SSBehaviorTreeFactory& factory)
    {
        const QtNodes::NodeDataModel* node_model = node->nodeDataModel();
        const std::string model_name = node_model->name().toStdString();

        const auto* bt_node = dynamic_cast<const Models::SSBehaviorTreeNodeDataModel*>(node_model);

        QString registration_name = bt_node->registrationName();
        QDomElement element;

        if (BuiltInNodeModels(factory).count(registration_name) != 0)
        {
            element = doc.createElement(registration_name.toStdString().c_str());
        }
        else
        {
            element = doc.createElement(QString::fromStdString(BT::toStr(bt_node->nodeType())));
            element.setAttribute("ID", registration_name.toStdString().c_str());
        }

        bool is_subtree_expanded = false;
        if (auto subtree = dynamic_cast<const Models::SubtreeNodeModel*>(node_model))
        {
            is_subtree_expanded = subtree->expanded();
        }

        if (bt_node->instanceName() != registration_name)
        {
            element.setAttribute("name", bt_node->instanceName().toStdString().c_str());
        }

        auto port_mapping = bt_node->getCurrentPortMapping();
        for (const auto& port_it : port_mapping)
        {
            element.setAttribute(port_it.first, port_it.second);
        }

        parent_element.appendChild(element);

        if (!is_subtree_expanded)
        {
            auto node_children = getChildren(scene, *node, true);
            for (const QtNodes::Node* child : node_children)
            {
                RecursivelyCreateXml(scene, doc, element, child, factory);
            }
        }
    }

    bool VerifyXML(QDomDocument& doc, const std::vector<QString>& registered_ID, std::vector<QString>& error_messages)
    {
        error_messages.clear();
        try
        {
            std::string xml_text = doc.toString().toStdString();
            std::set<std::string> registered_nodes;

            for (const auto& str : registered_ID)
            {
                registered_nodes.insert(str.toStdString());
            }

            BT::VerifyXML(xml_text, registered_nodes); // may throw
        } catch (std::exception& ex)
        {
            error_messages.push_back(ex.what());
        }
        return true;
    }

    QDomElement writePortModel(const QString& port_name, const PortModel& port, QDomDocument& doc)
    {
        QDomElement port_element;
        switch (port.direction)
        {
        case PortDirection::INPUT:
            port_element = doc.createElement("input_port");
            break;
        case PortDirection::OUTPUT:
            port_element = doc.createElement("output_port");
            break;
        case PortDirection::INOUT:
            port_element = doc.createElement("inout_port");
            break;
        }

        port_element.setAttribute("name", port_name);
        if (port.typeName.isEmpty() == false)
        {
            port_element.setAttribute("type", port.typeName);
        }
        if (port.defaultValue.isEmpty() == false)
        {
            port_element.setAttribute("default", port.defaultValue);
        }

        if (!port.description.isEmpty())
        {
            QDomText description = doc.createTextNode(port.description);
            port_element.appendChild(description);
        }
        return port_element;
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core
