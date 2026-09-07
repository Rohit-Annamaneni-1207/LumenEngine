#include "bindings.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "lumen/core/image_view.hpp"
#include "lumen/ops/point.hpp"

#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace py = pybind11;

namespace lumen::python {
    namespace {

        // POINT INVERSION BINDING
        py::array_t<std::uint8_t> invert_array (
            py::array_t<std::uint8_t, py::array::c_style> source
        ) {
            const py::buffer_info source_info = source.request();

            if (source_info.ndim != 2 && source_info.ndim != 3) {
                throw std::invalid_argument(
                    "image must have shape (height, width) or "
                    "(height, width, channels)"
                );
            }

            const std::size_t height = static_cast<std::size_t>(source_info.shape[0]);

            const std::size_t width = static_cast<std::size_t>(source_info.shape[1]);
        
            const std::size_t channels = source_info.ndim == 2 ? 1 : static_cast<std::size_t>(source_info.shape[2]);

            if (channels != 1 && channels != 3) {
                throw std::invalid_argument(
                    "image must be grayscale or RGB"
                );
            }


            py::array_t<std::uint8_t> destination(
                source_info.shape
            );

            const py::buffer_info destination_info = destination.request();

            const ImageView<const std::uint8_t> source_view(
                static_cast<const std::uint8_t*>(source_info.ptr),
                width,
                height,
                channels,
                static_cast<std::size_t>(source_info.strides[0])
            );

            ImageView<std::uint8_t> destination_view(
                static_cast<std::uint8_t*>(destination_info.ptr),
                width,
                height,
                channels,
                static_cast<std::size_t>(destination_info.strides[0])
            );

            {
                py::gil_scoped_release release;
                lumen::invert(source_view, destination_view);
            }

            return destination;
        }

        // ADJUST BRIGHTNESS BINDING
        py::array_t<std::uint8_t> adjust_brightness_array(
            py::array_t<std::uint8_t, py::array::c_style> source,
            int delta
        ) {
            const py::buffer_info source_info = source.request();

            if (source_info.ndim != 2 && source_info.ndim != 3) {
                throw std::invalid_argument(
                    "image must have shape (height, width) or "
                    "(height, width, channels)"
                );
            }

            const std::size_t height = static_cast<std::size_t>(source_info.shape[0]);
            const std::size_t width = static_cast<std::size_t>(source_info.shape[1]);
            const std::size_t channels = static_cast<std::size_t>(source_info.ndim == 2 ? 1 : source_info.shape[2]);

            if (channels != 1 && channels != 3)
            {
                throw std::invalid_argument("Only grayscale or RGB images are supported");
            }

            py::array_t<std::uint8_t> dest(source_info.shape);
            const py::buffer_info dest_info = dest.request();

            const ImageView<const std::uint8_t> source_view(
                static_cast<const std::uint8_t*>(source_info.ptr),
                width,
                height,
                channels,
                static_cast<std::size_t>(source_info.strides[0])
            );

            ImageView<std::uint8_t> dest_view(
                static_cast<std::uint8_t*>(dest_info.ptr),
                width,
                height,
                channels,
                static_cast<std::size_t>(dest_info.strides[0])
            );

            {
                py::gil_scoped_release release;
                lumen::adjust_brightness(source_view, dest_view, delta);
            }

            return dest;
        }

        // BINARY THRESHOLDING BINDING
        py::array_t<std::uint8_t> binary_threshold_array(
            py::array_t<std::uint8_t, py::array::c_style> source,
            std::uint8_t threshold,
            std::uint8_t max_val
        ) {

            const py::buffer_info source_info = source.request();

            if (source_info.ndim != 2)
            {
                throw std::invalid_argument("binary threshold requires a grayscale image "
                    "with shape (height, width)"
                );
            }

            const std::size_t height = static_cast<std::size_t>(source_info.shape[0]);
            const std::size_t width = static_cast<std::size_t>(source_info.shape[1]);

            constexpr std::size_t channels = 1;

            py::array_t<std::uint8_t> dest(source_info.shape);

            const py::buffer_info dest_info = dest.request();

            const ImageView<const std::uint8_t> source_view(
                static_cast<const std::uint8_t*>(source_info.ptr),
                width,
                height,
                channels,
                static_cast<std::size_t>(source_info.strides[0])
            );

            ImageView<std::uint8_t> dest_view(
                static_cast<std::uint8_t*>(dest_info.ptr),
                width,
                height,
                channels,
                static_cast<std::size_t>(dest_info.strides[0])
            );

            {
                py::gil_scoped_release release;
                lumen::binary_threshold(
                    source_view,
                    dest_view,
                    threshold,
                    max_val
                );
            }

            return dest;
        }
    } // Bindings

    void bind_point(py::module_& module) {
        module.def(
            "invert",
            &invert_array,
            py::arg("image"),
            "Invert an 8-bit grayscale or RGB image"
        );

        module.def(
            "adjust_brightness",
            &adjust_brightness_array,
            py::arg("image"),
            py::arg("delta"),
            "Adjust image using a signed offset (delta)"
        );

        module.def(
            "binary_threshold",
            &binary_threshold_array,
            py::arg("image"),
            py::arg("threshold"),
            py::arg("max_value") = 255
        );
    } // Module defs


} //Lumen namespace
