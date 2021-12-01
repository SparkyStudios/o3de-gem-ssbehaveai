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

#include <Editor/Application/precompiled.h>

#include <QComboBox>
#include <QDropEvent>
#include <QLineEdit>
#include <QMimeData>

#include <Editor/Widgets/NodeProperties.h>
#include <Editor/Widgets/ui_NodeProperties.h>

#include <Editor/Core/EditorBase.h>
#include <Editor/Core/EditorUtils.h>
#include <Editor/Core/Models/SSBehaviorTreeNodeDataModel.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    NodeProperties::NodeProperties(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NodeProperties())
    {
        ui->setupUi(this);

        ui->formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        ui->formLayout->setHorizontalSpacing(4);
        ui->formLayout->setVerticalSpacing(2);
        ui->formLayout->setContentsMargins(0, 0, 0, 0);
        ui->formLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
    }

    NodeProperties::~NodeProperties()
    {
        delete ui;
    }

    void NodeProperties::SetNode(QtNodes::Node* node)
    {
        if (_currentNode)
        {
            QLayoutItem* item;
            while ((item = ui->formLayout->takeAt(0)))
            {
                if (item->widget())
                {
                    delete item->widget();
                }

                delete item;
            }
        }

        if (node)
        {
            auto* bt_model = dynamic_cast<Core::Models::SSBehaviorTreeNodeDataModel*>(node->nodeDataModel());
            Core::PortsMapping portsMapping = bt_model->getCurrentPortMapping();
            const Core::NodeModel& model = bt_model->model();

            BT::PortDirection preferred_port_types[3] = { BT::PortDirection::INPUT, BT::PortDirection::OUTPUT, BT::PortDirection::INOUT };

            auto* node_instance_name_field = new QLineEdit();
            node_instance_name_field->setText(bt_model->instanceName());

            auto* node_instance_name_label = new QLabel("Instance Name");

            ui->formLayout->addRow(node_instance_name_label, node_instance_name_field);

            connect(
                node_instance_name_field, &QLineEdit::textChanged, bt_model, &Core::Models::SSBehaviorTreeNodeDataModel::setInstanceName);

            for (int pref_index = 0; pref_index < 3; pref_index++)
            {
                for (const auto& port_it : model.ports)
                {
                    auto preferred_direction = preferred_port_types[pref_index];
                    if (port_it.second.direction != preferred_direction)
                    {
                        continue;
                    }

                    QString description = port_it.second.description;
                    QString label = port_it.first;
                    if (preferred_direction == BT::PortDirection::INPUT)
                    {
                        label.prepend("[IN] ");
                        if (description.isEmpty())
                        {
                            description = "[INPUT]";
                        }
                        else
                        {
                            description.prepend("[INPUT]: ");
                        }
                    }
                    else if (preferred_direction == BT::PortDirection::OUTPUT)
                    {
                        label.prepend("[OUT] ");
                        if (description.isEmpty())
                        {
                            description = "[OUTPUT]";
                        }
                        else
                        {
                            description.prepend("[OUTPUT]: ");
                        }
                    }

                    auto* form_field = new QLineEdit();
                    form_field->setAcceptDrops(true);

                    if (auto findIt = portsMapping.find(port_it.first); findIt != portsMapping.end())
                    {
                        form_field->setText(findIt->second);
                    }
                    else
                    {
                        form_field->setText(port_it.second.defaultValue);
                    }

                    QLabel* form_label = new QLabel(label, this);
                    form_label->setToolTip(description);

                    ui->formLayout->addRow(form_label, form_field);

                    auto paramUpdated = [port_it, form_field, bt_model]()
                    {
                        bt_model->setPortMapping(port_it.first, form_field->text());
                    };

                    if (auto lineedit = dynamic_cast<QLineEdit*>(form_field))
                    {
                        connect(lineedit, &QLineEdit::editingFinished, this, paramUpdated);
                        connect(
                            lineedit, &QLineEdit::editingFinished, bt_model, &Core::Models::SSBehaviorTreeNodeDataModel::updateNodeSize);
                    }
                    else if (auto combo = dynamic_cast<QComboBox*>(form_field))
                    {
                        connect(combo, &QComboBox::currentTextChanged, this, paramUpdated);
                    }
                }
            }

            _currentNode = node;
        }
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

#include <Editor/Widgets/moc_NodeProperties.cpp>
