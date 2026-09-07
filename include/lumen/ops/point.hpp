#pragma once

#include "lumen/core/image_view.hpp"
#include <cstdint>

namespace lumen {
    void invert(
        ImageView<const std::uint8_t> source,
        ImageView<std::uint8_t> dest
    );

    void adjust_brightness(
        ImageView<const std::uint8_t> source,
        ImageView<std::uint8_t> dest,
        int delta
    );

    void binary_threshold(
        ImageView<const std::uint8_t> source,
        ImageView<std::uint8_t> dest,
        std::uint8_t threshold,
        std::uint8_t max_val = 255
    );
}