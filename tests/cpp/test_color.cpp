#include "lumen/core/image_view.hpp"
#include "lumen/ops/color.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>

void test_rgb_to_grayscale() {
    const std::size_t width = 5;
    const std::size_t height = 1;

    const std::vector<std::uint8_t> source_pixels{
        0, 0, 0,
        255, 255, 255,
        255, 0, 0,
        0, 255, 0,
        0, 0, 255
    };
    std::vector<std::uint8_t> dest_pixels(width * height);

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(),
        width,
        height,
        3,
        width * 3
    );

    lumen::ImageView<std::uint8_t> dest(
        dest_pixels.data(),
        width,
        height,
        1,
        width
    );

    lumen::rgb_to_grayscale(source, dest);

    const std::vector<std::uint8_t> expected{
        0, 255, 76, 150, 29
    };

    assert(dest_pixels == expected);
}

void test_rgb_to_grayscale_rejects_non_rgb_source() {
    const std::vector<std::uint8_t> source_pixels{100};
    std::vector<std::uint8_t> dest_pixels(1);

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(),
        1,
        1,
        1,
        1
    );

    lumen::ImageView<std::uint8_t> dest(
        dest_pixels.data(),
        1,
        1,
        1,
        1
    );

    bool threw = false;

    try {
        lumen::rgb_to_grayscale(source, dest);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);
}

void test_rgb_to_grayscale_rejects_multichannel_destination() {
    const std::vector<std::uint8_t> source_pixels{10, 20, 30};
    std::vector<std::uint8_t> dest_pixels(3);

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(),
        1,
        1,
        3,
        3
    );

    lumen::ImageView<std::uint8_t> dest(
        dest_pixels.data(),
        1,
        1,
        3,
        3
    );

    bool threw = false;

    try {
        lumen::rgb_to_grayscale(source, dest);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);
}

int main() {
    test_rgb_to_grayscale();
    test_rgb_to_grayscale_rejects_non_rgb_source();
    test_rgb_to_grayscale_rejects_multichannel_destination();

    return 0;
}
