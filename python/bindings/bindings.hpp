#pragma once

#include <pybind11/pybind11.h>

namespace lumen::python {

void bind_point(pybind11::module_& module);
void bind_color(pybind11::module_& module);

}  // namespace lumen::python