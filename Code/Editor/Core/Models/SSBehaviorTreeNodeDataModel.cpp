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

#include <Editor/Core/Models/SSBehaviorTreeNodeDataModel.h>

#include <QBoxLayout>
#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QFormLayout>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QSizePolicy>

namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core::Models
{
    static int GetUID()
    {
        static int uid = 1000;
        return uid++;
    }

    const int MARGIN = 10;
    const int DEFAULT_LINE_WIDTH = 100;
    const int DEFAULT_FIELD_WIDTH = 50;
    const int DEFAULT_LABEL_WIDTH = 50;

    SSBehaviorTreeNodeDataModel::SSBehaviorTreeNodeDataModel(const NodeModel& model)
        : _uid(GetUID())
        , _model(model)
        , _instance_name(model.registrationId)
    {
    }

    void SSBehaviorTreeNodeDataModel::initWidget()
    {
        updateNodeSize();
    }

    QJsonObject SSBehaviorTreeNodeDataModel::save() const
    {
        QJsonObject modelJson;

        modelJson["name"] = registrationName();
        modelJson["alias"] = instanceName();

        return modelJson;
    }

    BT::NodeType SSBehaviorTreeNodeDataModel::nodeType() const
    {
        return _model.type;
    }

    unsigned int SSBehaviorTreeNodeDataModel::nPorts(QtNodes::PortType portType) const
    {
        if (portType == QtNodes::PortType::Out)
        {
            if (nodeType() == NodeType::ACTION || nodeType() == NodeType::CONDITION)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if (portType == QtNodes::PortType::In)
        {
            return (_model.registrationId == "Root") ? 0 : 1;
        }

        return 0;
    }

    QtNodes::NodeDataModel::ConnectionPolicy SSBehaviorTreeNodeDataModel::portOutConnectionPolicy(QtNodes::PortIndex) const
    {
        return (nodeType() == NodeType::DECORATOR || _model.registrationId == "Root") ? ConnectionPolicy::One : ConnectionPolicy::Many;
    }

    QtNodes::NodeDataType SSBehaviorTreeNodeDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
    {
        return {};
    }

    std::shared_ptr<QtNodes::NodeData> SSBehaviorTreeNodeDataModel::outData(QtNodes::PortIndex)
    {
        return nullptr;
    }

    void SSBehaviorTreeNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, [[maybe_unused]] QtNodes::PortIndex portIndex)
    {
    }

    PortsMapping SSBehaviorTreeNodeDataModel::getCurrentPortMapping() const
    {
        return _ports_mapping;
    }

    void SSBehaviorTreeNodeDataModel::setInstanceName(const QString& name)
    {
        AZ_UNUSED(name);

        _instance_name = name;

        updateNodeSize();
        emit instanceNameChanged();
    }

    void SSBehaviorTreeNodeDataModel::lock(bool locked)
    {
        AZ_UNUSED(locked);

        // _line_edit_name->setEnabled(!locked);

        // TODO: Mark item as locked
    }

    void SSBehaviorTreeNodeDataModel::setPortMapping([[maybe_unused]] const QString& port_name, [[maybe_unused]] const QString& value)
    {
        if (_ports_mapping.count(port_name) == 0)
        {
            _ports_mapping.insert({ port_name, value });
        }
        else
        {
            _ports_mapping[port_name] = value;
        }
    }

    void SSBehaviorTreeNodeDataModel::updateNodeSize()
    {
        // int caption_width = _caption_label->width();
        // caption_width += _caption_logo_left->width() + _caption_logo_right->width();
        // int line_edit_width = caption_width;

        // if (_line_edit_name->isHidden() == false)
        // {
        //     QFontMetrics fm = _line_edit_name->fontMetrics();
        //     const QString& txt = _line_edit_name->text();
        //     int text_width = fm.boundingRect(txt).width();
        //     line_edit_width = std::max(line_edit_width, text_width + MARGIN);
        // }

        // //----------------------------
        // int field_colum_width = DEFAULT_LABEL_WIDTH;
        // int label_colum_width = 0;

        // field_colum_width = std::max(field_colum_width, line_edit_width - label_colum_width);
        // line_edit_width = std::max(line_edit_width, label_colum_width + field_colum_width);

        // _line_edit_name->setFixedWidth(line_edit_width);

        // _main_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        // _main_widget->adjustSize();

        //----------------------------

        emit embeddedWidgetSizeUpdated();
    }

    void SSBehaviorTreeNodeDataModel::onHighlightPortValue(QString value)
    {
    }

    QtNodes::NodeValidationState SSBehaviorTreeNodeDataModel::validationState() const
    {
        if (instanceName() == registrationName())
            return QtNodes::NodeValidationState::Valid;

        return QtNodes::NodeValidationState::Warning;
    }

    QString SSBehaviorTreeNodeDataModel::validationMessage() const
    {
        return registrationName();
    }

    const QString& SSBehaviorTreeNodeDataModel::registrationName() const
    {
        return _model.registrationId;
    }

    const QString& SSBehaviorTreeNodeDataModel::instanceName() const
    {
        return _instance_name;
    }

} // namespace SparkyStudios::AI::Behave::BehaviorTree::Editor::Core::Models

#include <Editor/Core/Models/moc_SSBehaviorTreeNodeDataModel.cpp>
