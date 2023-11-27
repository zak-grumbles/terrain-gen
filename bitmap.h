#ifndef BITMAP_H
#define BITMAP_H

#include <QDataStream>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

struct BitmapHeader {
    char bmp_signature_bytes[2] = {'B', 'M'};
    const uint32_t header_size  = 14;
    uint32_t size               = 0;
    uint32_t reserved_bytes     = 0;
    uint32_t data_offset        = 54;

    friend std::ostream& operator<<(
        std::ostream& os, const BitmapHeader& header
    );
    friend QDataStream& operator<<(
        QDataStream& stream, const BitmapHeader& header
    );
};

struct BitmapInfo {
    const uint32_t info_size          = 40;  // Constant size
    int32_t width                     = 0;
    int32_t height                    = 0;
    const uint16_t num_color_planes   = 1;  // Constant
    uint16_t color_depth              = 32;
    const uint32_t compression_method = 0;
    uint32_t raw_bmp_data_size        = 0;  // usually ignored
    int32_t horizontal_resolution     = 3780;
    int32_t vertical_resolution       = 3780;
    uint32_t color_table_entries      = 0;
    uint32_t important_colors         = 0;

    friend std::ostream& operator<<(std::ostream& os, const BitmapInfo& header);
    friend QDataStream& operator<<(
        QDataStream& stream, const BitmapInfo& header
    );
};

struct BitmapPixel {
    uint8_t blue  = 255;
    uint8_t green = 255;
    uint8_t red   = 0;
    uint8_t alpha = 0;

    friend std::ostream& operator<<(std::ostream& os, const BitmapPixel& pxl);
    friend QDataStream& operator<<(QDataStream& stream, const BitmapPixel& pxl);
};

class Bitmap {
public:
    Bitmap();
    Bitmap(int width, int height);

    ~Bitmap();

    void GivePixelData(const std::vector<BitmapPixel>& data);

    std::vector<BitmapPixel> Data() const { return pixel_data_; }

    friend std::ostream& operator<<(std::ostream& os, const Bitmap& bmp);
    friend QDataStream& operator<<(QDataStream& stream, const Bitmap& bmp);

protected:
    BitmapInfo bmp_info_;
    BitmapHeader bmp_header_;
    std::vector<BitmapPixel> pixel_data_;
};

#endif  // BITMAP_H
