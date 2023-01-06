#include "bitmap.h"

std::ostream& operator<<(std::ostream& os, const BitmapHeader& header)
{
    os.write(header.bmp_signature_bytes, 2);
    os.write((char*)&header.size, sizeof(uint32_t));
    os.write((char*)&header.reserved_bytes, sizeof(uint32_t));
    os.write((char*)&header.data_offset, sizeof(uint32_t));
    return os;
}

QDataStream& operator<<(QDataStream& stream, const BitmapHeader& header)
{
    stream.writeRawData(header.bmp_signature_bytes, 2);
    stream.writeRawData((char*)&header.size, sizeof(uint32_t));
    stream.writeRawData((char*)&header.reserved_bytes, sizeof(uint32_t));
    stream.writeRawData((char*)&header.data_offset, sizeof(uint32_t));
    return stream;
}

std::ostream& operator<<(std::ostream& os, const BitmapInfo& info)
{
    os.write((char*)&info.info_size, sizeof(uint32_t));
    os.write((char*)&info.width, sizeof(uint32_t));
    os.write((char*)&info.height, sizeof(uint32_t));
    os.write((char*)&info.num_color_planes, sizeof(uint16_t));
    os.write((char*)&info.color_depth, sizeof(uint16_t));
    os.write((char*)&info.compression_method, sizeof(uint32_t));
    os.write((char*)&info.raw_bmp_data_size, sizeof(uint32_t));
    os.write((char*)&info.horizontal_resolution, sizeof(int32_t));
    os.write((char*)&info.vertical_resolution, sizeof(int32_t));
    os.write((char*)&info.color_table_entries, sizeof(uint32_t));
    os.write((char*)&info.important_colors, sizeof(uint32_t));

    return os;
}

QDataStream& operator<<(QDataStream& stream, const BitmapInfo& info)
{
    stream.writeRawData((char*)&info.info_size, sizeof(uint32_t));
    stream.writeRawData((char*)&info.width, sizeof(uint32_t));
    stream.writeRawData((char*)&info.height, sizeof(uint32_t));
    stream.writeRawData((char*)&info.num_color_planes, sizeof(uint16_t));
    stream.writeRawData((char*)&info.color_depth, sizeof(uint16_t));
    stream.writeRawData((char*)&info.compression_method, sizeof(uint32_t));
    stream.writeRawData((char*)&info.raw_bmp_data_size, sizeof(uint32_t));
    stream.writeRawData((char*)&info.horizontal_resolution, sizeof(int32_t));
    stream.writeRawData((char*)&info.vertical_resolution, sizeof(int32_t));
    stream.writeRawData((char*)&info.color_table_entries, sizeof(uint32_t));
    stream.writeRawData((char*)&info.important_colors, sizeof(uint32_t));
    return stream;
}

std::ostream& operator<<(std::ostream& os, const BitmapPixel& pxl)
{
    os.write((char*)&pxl.blue, sizeof(uint8_t));
    os.write((char*)&pxl.green, sizeof(uint8_t));
    os.write((char*)&pxl.red, sizeof(uint8_t));
    os.write((char*)&pxl.alpha, sizeof(uint8_t));
    return os;
}

QDataStream& operator<<(QDataStream& stream, const BitmapPixel& pxl)
{
    stream.writeRawData((char*)&pxl.blue, sizeof(uint8_t));
    stream.writeRawData((char*)&pxl.green, sizeof(uint8_t));
    stream.writeRawData((char*)&pxl.red, sizeof(uint8_t));
    stream.writeRawData((char*)&pxl.alpha, sizeof(uint8_t));
    return stream;
}

Bitmap::Bitmap() : Bitmap(0,0)
{

}

Bitmap::Bitmap(int width, int height)
{
    bmp_info_.width = width;
    bmp_info_.height = height;
}

Bitmap::~Bitmap()
{
    pixel_data_.clear();
}

void Bitmap::GivePixelData(const std::vector<BitmapPixel>& data)
{
    if(!pixel_data_.empty())
    {
        pixel_data_.clear();
        pixel_data_.reserve(data.size());
    }

    pixel_data_ = data;
    bmp_header_.size = bmp_header_.header_size + bmp_info_.info_size;
    bmp_header_.size += pixel_data_.size() * (sizeof(uint8_t) * 4);
}

std::ostream& operator<<(std::ostream& os, const Bitmap& bmp)
{
    os << bmp.bmp_header_ << bmp.bmp_info_;

    for(const BitmapPixel& p : bmp.pixel_data_)
    {
        os << p;
    }

    return os;
}

QDataStream& operator<<(QDataStream& stream, const Bitmap& bmp)
{
    stream << bmp.bmp_header_ << bmp.bmp_info_;

    for(const BitmapPixel& p : bmp.pixel_data_)
    {
        stream << p;
    }

    return stream;
}
