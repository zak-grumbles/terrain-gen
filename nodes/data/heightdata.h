#ifndef HEIGHTDATA_H
#define HEIGHTDATA_H

#include <QtNodes/NodeData>

/*!
 * \class HeightData
 * \brief Abstract class used for any node data type that
 * can be used as height data.
 *
 * \ingroup Nodes
 * \ingroup Node Data
 */
class HeightData : public QtNodes::NodeData
{
public:
    /*!
     * \enum HeightDataType
     *
     * This enum describes what the sub-type
     * of a given HeightData object is.
     *
     * \value kFastNoise The HeightData is of sub-type NoiseData
     * \value kHeightmap The HeightData is of sub-type HeightmapData
     */
    enum HeightDataType
    {
        kFastNoise = 0,
        kHeightmap,
        kOperation,
        kNone
    };

    QtNodes::NodeDataType type() const override;

    virtual float GetValueAt(float x, float y) const { return 0.0f; }

    QPixmap* AsBitmap(float x_offset, float y_offset,
                    float width, float height);
    QPixmap* AsBitmap(QSize offset, QSize size);


protected:
    HeightDataType type_ = HeightDataType::kNone;
};

#endif // HEIGHTDATA_H
