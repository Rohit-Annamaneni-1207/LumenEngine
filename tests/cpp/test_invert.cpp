#include "lumen/core/image_view.hpp"
#include "lumen/ops/point.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

int main()
{
    const std::size_t width = 3;
    const std::size_t height = 2;
    const std::size_t channels = 1;
    const std::size_t row_stride = width * channels;

    std::vector<std::uint8_t> source_pixels{0, 10, 255, 50, 100, 200};
    std::vector<std::uint8_t> dest_pixels(source_pixels.size());

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(), 
        width, 
        height, 
        channels, 
        row_stride
    );

    const lumen::ImageView<std::uint8_t> dest(
        dest_pixels.data(), 
        width, 
        height, 
        channels, 
        row_stride
    );

    lumen::invert(source, dest);

    const std::vector<std::uint8_t> expected{
        255, 245, 0,
        205, 155, 55
    };

    assert(dest_pixels == expected);
    assert(source_pixels[0] == 0);

    return 0;
}