#ifndef IMAGESOURCEDATA_H
#define IMAGESOURCEDATA_H

#include <QPixmap>
#include <QtNodes/NodeData>

#include "heightdata.h"

class ImageSourceData : public HeightData
{
public:
    ImageSourceData();

    virtual ~ImageSourceData() { image_.reset(); }

    HeightDataType GetHeightDataType() const override { return HeightDataType::kImageSource; }

    void SetImage(std::shared_ptr<QPixmap> new_image);

    bool HasImage() const { return image_ != nullptr; }

protected:
    std::shared_ptr<QPixmap> image_ = nullptr;
};

#endif // IMAGESOURCEDATA_H
