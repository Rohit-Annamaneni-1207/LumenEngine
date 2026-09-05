#include <pybind11/pybind11.h>

#include "lumen/version.hpp"

#include "bindings.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_lumen_native, module) {
    module.doc() = "Native bindings for LumenEngine";

    module.def(
        "version_major",
        &lumen::version_major,
        "Return LumenEngine's major version"
    );

    lumen::python::bind_point(module);
}