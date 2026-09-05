cmake_minimum_required(VERSION 3.20...4.4)

project(
    LumenEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

add_library(
    lumen_core
    STATIC
    src/core/version.cpp
    src/ops/point.cpp
)

target_include_directories(
    lumen_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(
    lumen_core
    PUBLIC
        cxx_std_17
)

set_target_properties(
    lumen_core
    PROPERTIES
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)

if(APPLE OR UNIX)
    target_compile_options(
        lumen_core
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

option(
    LUMEN_BUILD_PYTHON
    "Build the LumenEngine Python extension"
    OFF
)

if(LUMEN_BUILD_PYTHON)
    set(PYBIND11_FINDPYTHON ON)

    find_package(
        pybind11
        CONFIG
        REQUIRED
    )

    pybind11_add_module(
        _lumen_native
        python/bindings/module.cpp
        python/bindings/bind_point.cpp
    )

    target_link_libraries(
        _lumen_native
        PRIVATE
            lumen_core
    )

    install(
        TARGETS _lumen_native
        LIBRARY DESTINATION lumen
    )
endif()

include(CTest)

if(BUILD_TESTING)
    add_executable(
        lumen_core_tests
        tests/cpp/test_version.cpp
    )

    target_link_libraries(
        lumen_core_tests
        PRIVATE
            lumen_core
    )

    add_test(
        NAME lumen.version
        COMMAND lumen_core_tests
    )

    add_executable(
        lumen_point_tests
        tests/cpp/test_invert.cpp
    )

    target_link_libraries(
        lumen_point_tests
        PRIVATE
            lumen_core
    )

    add_test(
        NAME lumen.point
        COMMAND lumen_point_tests
    )    
endif()