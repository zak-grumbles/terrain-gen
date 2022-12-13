#ifndef NUMBERSOURCEDATAMODEL_H
#define NUMBERSOURCEDATAMODEL_H

#include "nodes/integerdata.h"
#include "qspinbox.h"

#include <QObject>
#include <QtNodes/NodeDelegateModel>

class NumberSourceDataModel : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    NumberSourceDataModel();
    virtual ~NumberSourceDataModel() {}

    QString caption() const override { return QStringLiteral("Number Source"); }

    bool captionVisible() const override { return false; }

    QString name() const override { return QStringLiteral("NumberSource"); }

    QJsonObject save() const override;
    void load(QJsonObject const& obj) override;

    unsigned int nPorts(QtNodes::PortType port_type) const override;

    QtNodes::NodeDataType dataType(
            QtNodes::PortType port_type,
            QtNodes::PortIndex port_index) const override;

    std::shared_ptr<QtNodes::NodeData> outData(
            QtNodes::PortIndex port) override;

    void setInData(
            std::shared_ptr<QtNodes::NodeData> data,
            QtNodes::PortIndex index) override
    {}

    QWidget* embeddedWidget() override;

protected slots:
    void OnValueChanged_(int new_val);

protected:

    std::shared_ptr<IntegerData> data_;

    QSpinBox* spin_box_;
};

#endif // NUMBERSOURCEDATAMODEL_H
