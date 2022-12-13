#ifndef INTEGERDATA_H
#define INTEGERDATA_H

#include <QtNodes/NodeData>

class IntegerData : public QtNodes::NodeData
{
public:
    IntegerData() : IntegerData(0)
    {}

    IntegerData(int const value) : value_(value)
    {}

    QtNodes::NodeDataType type() const override;

    int value() const { return value_; }

    QString ValueAsText() const { return QString::number(value_); }

    void SetValue(int new_value) { value_ = new_value; }

protected:

    int value_ = 0;
};

#endif // INTEGERDATA_H
