#pragma once

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>
#include <Editor/Core/EditorBase.h>

#include <QEvent>
#include <QFile>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QSvgRenderer>
#endif

namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models
{
    class SSBehaviorTreeNodeDataModel : public QtNodes::NodeDataModel
    {
        Q_OBJECT

    public:
        SSBehaviorTreeNodeDataModel(const NodeModel& model);
        virtual ~SSBehaviorTreeNodeDataModel() = default;

        virtual void setInstanceName(const QString& name);

        QString caption() const override
        {
            if (instanceName() == registrationName())
                return registrationName();

            return instanceName();
        }

        bool captionVisible() const override
        {
            return true;
        }

        bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override
        {
            return false;
        }

        QString portCaption([[maybe_unused]] QtNodes::PortType portType, [[maybe_unused]] QtNodes::PortIndex portIndex) const override
        {
            return QString();
        }

        const QString& registrationName() const;

        const QString& instanceName() const;

        PortsMapping getCurrentPortMapping() const;

        const NodeModel& model() const
        {
            return _model;
        }

        void lock(bool locked);

        void setPortMapping(const QString& port_name, const QString& value);

        int UID() const
        {
            return _uid;
        }

        QString name() const override
        {
            return registrationName();
        }

        QJsonObject save() const override;

        BT::NodeType nodeType() const;

        ConnectionPolicy portOutConnectionPolicy(QtNodes::PortIndex) const final;

        unsigned int nPorts(QtNodes::PortType portType) const override;

        QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

        void setInData(std::shared_ptr<QtNodes::NodeData>, int) override;

        QWidget* embeddedWidget() override
        {
            return nullptr;
        }

        QtNodes::NodeValidationState validationState() const override;

        QString validationMessage() const override;

        void initWidget();

    public slots:
        void updateNodeSize();
        void onHighlightPortValue(QString value);

    signals:
        void instanceNameChanged();

    protected:
        int16_t _uid;

    private:
        PortsMapping _ports_mapping;

        const NodeModel _model;
        QString _instance_name;
        QSvgRenderer* _icon_renderer;

        QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Valid;
        QString modelValidationError = QStringLiteral("");
    };

} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core::Models
