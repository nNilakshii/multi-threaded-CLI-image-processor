#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <cstdint>
#include <string>
#include <vector>

struct Pixel {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

class BMPImage {
public:
    void load(const std::string& path);
    void save(const std::string& path) const;
    void applyGrayscale();

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }
    const std::vector<Pixel>& pixels() const noexcept { return pixels_; }
    std::vector<Pixel>& pixels() noexcept { return pixels_; }

private:
#pragma pack(push, 1)
    struct BMPFileHeader {
        std::uint16_t bfType;
        std::uint32_t bfSize;
        std::uint16_t bfReserved1;
        std::uint16_t bfReserved2;
        std::uint32_t bfOffBits;
    };

    struct BMPInfoHeader {
        std::uint32_t biSize;
        std::int32_t biWidth;
        std::int32_t biHeight;
        std::uint16_t biPlanes;
        std::uint16_t biBitCount;
        std::uint32_t biCompression;
        std::uint32_t biSizeImage;
        std::int32_t biXPelsPerMeter;
        std::int32_t biYPelsPerMeter;
        std::uint32_t biClrUsed;
        std::uint32_t biClrImportant;
    };
#pragma pack(pop)

    static constexpr std::uint16_t expectedMagic = 0x4D42; // 'BM'
    static constexpr std::uint16_t expectedBitCount = 24;
    static constexpr std::uint32_t compressionNone = 0;

    BMPFileHeader fileHeader_{};
    BMPInfoHeader infoHeader_{};
    int width_ = 0;
    int height_ = 0;
    bool topDown_ = false;
    std::vector<Pixel> pixels_;
};

#endif // BMPIMAGE_H
