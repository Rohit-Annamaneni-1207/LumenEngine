#include "lumen/core/image_view.hpp"
#include "lumen/ops/point.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>

void test_invert() {
    const std::size_t width = 3;
    const std::size_t height = 2;
    const std::size_t channels = 1;
    const std::size_t row_stride = width * channels;

    const std::vector<std::uint8_t> source_pixels{
        0, 10, 255,
        50, 100, 200
    };
    std::vector<std::uint8_t> dest_pixels(source_pixels.size());

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(),
        width,
        height,
        channels,
        row_stride
    );

    lumen::ImageView<std::uint8_t> dest(
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
}

void test_adjust_brightness_positive_delta() {
    const std::size_t width = 5;
    const std::size_t height = 1;
    const std::size_t channels = 1;
    const std::size_t row_stride = width * channels;

    const std::vector<std::uint8_t> source_pixels{
        0, 20, 100, 240, 255
    };
    std::vector<std::uint8_t> dest_pixels(source_pixels.size());

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(),
        width,
        height,
        channels,
        row_stride
    );

    lumen::ImageView<std::uint8_t> dest(
        dest_pixels.data(),
        width,
        height,
        channels,
        row_stride
    );

    lumen::adjust_brightness(source, dest, 30);

    const std::vector<std::uint8_t> expected{
        30, 50, 130, 255, 255
    };

    assert(dest_pixels == expected);
}

void test_adjust_brightness_negative_delta() {
    const std::size_t width = 5;
    const std::size_t height = 1;
    const std::size_t channels = 1;
    const std::size_t row_stride = width * channels;

    const std::vector<std::uint8_t> source_pixels{
        0, 20, 100, 240, 255
    };
    std::vector<std::uint8_t> dest_pixels(source_pixels.size());

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(),
        width,
        height,
        channels,
        row_stride
    );

    lumen::ImageView<std::uint8_t> dest(
        dest_pixels.data(),
        width,
        height,
        channels,
        row_stride
    );

    lumen::adjust_brightness(source, dest, -30);

    const std::vector<std::uint8_t> expected{
        0, 0, 70, 210, 225
    };

    assert(dest_pixels == expected);
}

void test_binary_threshold() {
    const std::size_t width = 5;
    const std::size_t height = 1;
    const std::size_t channels = 1;
    const std::size_t row_stride = width * channels;

    const std::vector<std::uint8_t> source_pixels{
        0, 127, 128, 129, 255
    };
    std::vector<std::uint8_t> dest_pixels(source_pixels.size());

    const lumen::ImageView<const std::uint8_t> source(
        source_pixels.data(),
        width,
        height,
        channels,
        row_stride
    );

    lumen::ImageView<std::uint8_t> dest(
        dest_pixels.data(),
        width,
        height,
        channels,
        row_stride
    );

    lumen::binary_threshold(source, dest, 128);

    const std::vector<std::uint8_t> expected_default{
        0, 0, 0, 255, 255
    };
    assert(dest_pixels == expected_default);

    lumen::binary_threshold(source, dest, 128, 1);

    const std::vector<std::uint8_t> expected_custom{
        0, 0, 0, 1, 1
    };
    assert(dest_pixels == expected_custom);
}

void test_binary_threshold_rejects_rgb() {
    const std::vector<std::uint8_t> source_pixels{10, 20, 30};
    std::vector<std::uint8_t> dest_pixels(source_pixels.size());

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
        lumen::binary_threshold(source, dest, 128);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);
}

int main() {
    test_invert();
    test_adjust_brightness_positive_delta();
    test_adjust_brightness_negative_delta();
    test_binary_threshold();
    test_binary_threshold_rejects_rgb();

    return 0;
}
