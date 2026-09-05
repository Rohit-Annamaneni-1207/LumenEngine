#include "lumen/ops/point.hpp"

#include <cstddef>
#include <stdexcept>

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
}