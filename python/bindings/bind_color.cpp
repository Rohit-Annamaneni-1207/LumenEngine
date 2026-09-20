#include "bindings.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "lumen/core/image_view.hpp"
#include "lumen/ops/color.hpp"

namespace py = pybind11;

namespace lumen::python {

    namespace {

        py::array_t<std::uint8_t> RBG_to_Grayscale_array (
            const py::array_t<std::uint8_t, py::array::c_style> source
        ) {

            py::buffer_info source_info = source.request();

            if (source_info.ndims != 3)
            {
                throw std::invalid_argument("Input for RGB to grayscale conversion must have exactly 3 dims");
            }

            if (source_info.shape[2] != 3)
            {
                throw std::invalid_argument("Input for RGB to grayscale conversion must have exactly 3 channels");
            }

            const std::size_t height = static_cast<std::size_t>(source.shape[0]);
            const std::size_t width = static_cast<std::size_t>(source.shape[1]);

            py::array_t<std::uint8_t> dest(source_info.shape[0], source_info.shape[1]);

            py::buffer_info dest_info = dest.request();

            const ImageView<const std::uint8_t> source_view (
                static_cast<const std::uint8_t*>(source_info.ptr),
                width,
                height,
                channels
                static_cast<std::size_t>(source_info.strides[0])
            );

            ImageView<std::uint8_t> dest_view (
                static_cast<std::uint8_t*>(dest_info.ptr),
                width,
                height,
                1,
                static_cast<std::size_t>(dest_info.strides[0])
            );

            {
                py::gil_scoped_release release;
                lumen::rgb_to_grayscale(source_view, dest_view);
            }

            return dest;
        }

    }

    void bind_point(py::module_& module)
    {
        module.def(
            "RGB_to_Grayscale",
            &RGB_to_Grayscale_array,
            py::arg("Image"),
            "Convert an RGB Image to grayscale"
        )
    }

}