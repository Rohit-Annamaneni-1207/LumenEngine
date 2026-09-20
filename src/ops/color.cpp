#include "lumen/ops/color.hpp"
// #include "lumen/core/image_view.hpp"

#include <cmath>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

namespace lumen {

    void rgb_to_grayscale(
        ImageView<const std::uint8_t> source,
        ImageView<std::uint8_t> dest
    ) {
        
        if (source.width() != dest.width() || source.height() != dest.height())
        {
            throw std::invalid_argument("Source and destination dims do not match.");
        }

        if (source.channels() != 3)
        {
            throw std::invalid_argument("Source must be RGB (3 channels)");
        }
        if (dest.channels() != 1)
        {
            throw std::invalid_argument("Destination must be grayscale (1 channel)");
        }

        for (std::size_t y = 0; y < source.height(); y++) {
            for (std::size_t x = 0; x < source.width(); x++) {

                const float red = static_cast<float>(source.at(x, y, 0));
                const float green = static_cast<float>(source.at(x, y, 1));
                const float blue = static_cast<float>(source.at(x, y, 2));

                const float grayscale = 0.299F*red + 0.587F*green + 0.114F*blue;

                dest.at(x, y) = static_cast<std::uint8_t>(std::round(grayscale));

            }
        }
    }

}