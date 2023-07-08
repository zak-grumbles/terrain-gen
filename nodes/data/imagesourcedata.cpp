#include "imagesourcedata.h"

ImageSourceData::ImageSourceData() : image_{nullptr}
{

}

void ImageSourceData::SetImage(std::shared_ptr<QPixmap> new_image)
{
    image_ = new_image;
}
