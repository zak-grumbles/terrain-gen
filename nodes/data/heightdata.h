#ifndef HEIGHTDATA_H
#define HEIGHTDATA_H

#include <QtNodes/NodeData>

enum class HeightDataType
{
    kNoiseSource = 0,
    kImageSource,
    kNone // Should likely never be used, but could indicate error
};

class HeightData : public QtNodes::NodeData
{
public:
    virtual ~HeightData() {}

    QtNodes::NodeDataType type() const override
    {
        return QtNodes::NodeDataType {
            "heightdata", "Height Data"
        };
    }

    virtual HeightDataType GetHeightDataType() const = 0;

protected:
};

#endif // HEIGHTDATA_H
