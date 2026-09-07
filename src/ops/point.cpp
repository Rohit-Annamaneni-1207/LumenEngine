#include "lumen/ops/point.hpp"

#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace lumen {
    void invert (
        ImageView<const std::uint8_t> source,
        ImageView<std::uint8_t> dest
    ) {
        if (source.width() != dest.width() || source.height() != dest.height() || source.channels() != dest.channels()) {
            throw std::invalid_argument("Source and destination dimensions do not match");
        }

        for (std::size_t y = 0; y < source.height(); y++) {
            for (std::size_t x = 0; x < source.width(); x++) {
                for (std::size_t c = 0; c < source.channels(); c++) {

                    dest.at(x, y, c) = static_cast<std::uint8_t>(255 - source.at(x, y, c));

                }
            }
        }
    }

    void adjust_brightness(
        ImageView<const std::uint8_t> source,
        ImageView<std::uint8_t> dest,
        int delta
    ) {
        if (source.width() != dest.width() || source.height() != dest.height() || source.channels() != dest.channels()) {
            throw std::invalid_argument("Source and destination dimensions do not match");
        }

        for (std::size_t y = 0; y < source.height(); y++) {
            for (std::size_t x = 0; x < source.width(); x++) {
                for (std::size_t c = 0; c < source.channels(); c++) {

                    const int adjusted = static_cast<int>(source.at(x, y, c)) + delta;

                    const int clamped = std::clamp(
                        adjusted,
                        0,
                        255
                    );

                    dest.at(x, y, c) =
                        static_cast<std::uint8_t>(clamped);

                }
            }
        }
    }
}