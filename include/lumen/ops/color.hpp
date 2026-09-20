#pragma once

#include "lumen/core/image_view.hpp"

#include <cstdint>

namespace lumen {


    void rgb_to_grayscale(
        ImageView<const std::uint8_t> source,
        ImageView<std::uint8_t> dest
    );

    

}