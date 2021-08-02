#include <Editor/Application/precompiled.h>

#include <Editor/Core/Models/SubtreeNodeModel.h>

#include <QLineEdit>
#include <QVBoxLayout>

namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models
{
    SubtreeNodeModel::SubtreeNodeModel(const NodeModel& model)
        : SSBehaviorTreeNodeDataModel(model)
        , _expanded(false)
    {
        _expand_button = new QPushButton(_expanded ? "Collapse" : "Expand");
        _expand_button->setMaximumWidth(100);

        _expand_button->setStyleSheet("QPushButton{"
                                      "  color: black; background-color: white; "
                                      "  border: 0px rgb(115, 210, 22);"
                                      "  padding: 4px; border-radius: 3px;}\n"
                                      "QPushButton:disabled { color: #303030; background-color: #a0a0a0; }");
        _expand_button->setFlat(false);
        _expand_button->setFocusPolicy(Qt::NoFocus);
        _expand_button->adjustSize();

        connect(
            _expand_button, &QPushButton::clicked, this,
            [this]()
            {
                emit expandButtonPushed();
            });

        updateNodeSize();
    }

    void SubtreeNodeModel::setExpanded(bool expand)
    {
        _expanded = expand;
        _expand_button->setText(_expanded ? "Collapse" : "Expand");
        _expand_button->adjustSize();
    }

    void SubtreeNodeModel::setInstanceName(const QString& name)
    {
        SSBehaviorTreeNodeDataModel::setInstanceName(name);
    }

    QJsonObject SubtreeNodeModel::save() const
    {
        QJsonObject modelJson;
        modelJson["name"] = registrationName();
        modelJson["alias"] = instanceName();
        modelJson["expanded"] = _expanded;

        return modelJson;
    }

    void SubtreeNodeModel::restore(const QJsonObject& modelJson)
    {
        if (registrationName() != modelJson["name"].toString())
        {
            AZ_Error("SSBehaviorTreeEditor", false, "error restoring: different registration_name");
            return;
        }

        QString alias = modelJson["alias"].toString();
        setInstanceName(alias);
        setExpanded(modelJson["expanded"].toBool());
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models

#include <Editor/Core/Models/moc_SubtreeNodeModel.cpp>
