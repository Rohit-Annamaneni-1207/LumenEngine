# LumenEngine Initial Plan

## Goal

Build a small, extensible image-processing engine in modern C++ that is exposed to Python as a NumPy-friendly module. The initial version should be achievable in two focused weekends and should establish the architecture needed to add more techniques without redesigning the library.

## Initial scope

Version 0.1 will support C-contiguous `uint8` NumPy arrays in these layouts:

- Grayscale: `(height, width)`
- Color: `(height, width, channels)` with three RGB channels

The initial operations are:

- RGB-to-grayscale conversion
- Brightness and contrast adjustment
- Binary thresholding
- Generic 2D convolution
- Box blur
- Gaussian blur
- Sobel gradients and gradient magnitude
- Nearest-neighbor resize
- Bilinear resize

Image decoding and encoding are intentionally outside the engine. Python examples will use Pillow to load and save images.

## Architecture principles

1. Keep the C++ engine independent of Python and NumPy.
2. Use non-owning image views at algorithm boundaries.
3. Express operations as ordinary functions rather than inheritance hierarchies.
4. Separate public operations from reusable implementation primitives.
5. Keep Python bindings thin: validate, adapt, call C++, and return.
6. Establish correctness before adding SIMD, threading, or GPU execution.
7. Preserve the public Python API when optimizing internal implementations.

## Planned repository structure

```text
LumenEngine/
├── CMakeLists.txt
├── pyproject.toml
├── README.md
├── plan.md
├── include/lumen/
│   ├── core/
│   │   ├── image.hpp
│   │   ├── image_view.hpp
│   │   ├── types.hpp
│   │   ├── border.hpp
│   │   └── numeric.hpp
│   └── ops/
│       ├── color.hpp
│       ├── point.hpp
│       ├── convolution.hpp
│       ├── filters.hpp
│       ├── gradients.hpp
│       └── resize.hpp
├── src/
│   ├── core/
│   └── ops/
├── python/
│   ├── bindings/
│   └── lumen/
├── tests/
│   ├── cpp/
│   └── python/
├── benchmarks/
└── examples/
```

## Core design

### Image storage and views

`Image<T>` will own an image buffer. `ImageView<T>` will provide non-owning access to an existing buffer and record:

- Data pointer
- Width and height
- Channel count
- Row stride

Algorithms will accept a read-only source view and a writable destination view. NumPy arrays can therefore be adapted to image views without copying their input data. The first Python API will allocate a new NumPy output for every operation, keeping ownership and lifetime handling straightforward.

### Shared primitives

The first reusable primitives will be:

- Shape and channel validation
- Border-coordinate handling
- Saturating conversion to `uint8`
- Pixel and channel traversal
- Generic convolution
- Separable convolution
- One-dimensional kernel generation
- Nearest-neighbor sampling
- Bilinear sampling

Higher-level operations should be assembled from these primitives where practical.

### Library boundaries

The build will produce two main targets:

- `lumen_core`: the standalone C++ image-processing library
- `_lumen_native`: the Python extension that links to `lumen_core`

The user-facing `lumen` Python package will re-export native operations and may provide lightweight convenience wrappers. It will not contain alternate algorithm implementations.

## Weekend 1: foundation and first operations

### Step 1: scaffold the project

- Add the CMake project and C++ library target.
- Add Python packaging and the native extension target.
- Add minimal C++ and Python test targets.
- Prove that `import lumen` works from a clean environment.

Deliverable: an installable package with a trivial native function and automated smoke test.

### Step 2: implement the image model

- Define pixel and dimension types.
- Implement `ImageView<T>`.
- Implement the owning `Image<T>` buffer.
- Add const-correct source and destination views.
- Test construction, indexing, strides, and invalid dimensions.

Deliverable: algorithms can operate on memory without depending on NumPy.

### Step 3: build the NumPy adapter

- Accept grayscale and RGB arrays.
- Validate datatype, dimensions, channel count, and contiguity.
- Adapt NumPy input memory to a const image view.
- Allocate NumPy output arrays and expose writable views to C++.
- Provide clear Python exceptions for invalid input.

Deliverable: a safe path from NumPy input through C++ to NumPy output.

### Step 4: add point and color operations

- RGB-to-grayscale conversion
- Brightness and contrast adjustment
- Binary thresholding
- Saturating conversion helpers

Deliverable: the first useful Python-visible image operations with C++ and Python tests.

### Step 5: add convolution

- Define supported border behavior.
- Implement generic 2D convolution.
- Implement box blur using convolution.
- Test tiny hand-calculated images, corners, and borders.

Deliverable: a reusable neighborhood-processing foundation.

## Weekend 2: filters, edges, and geometry

### Step 6: add separable Gaussian blur

- Generate normalized one-dimensional Gaussian kernels.
- Implement horizontal and vertical convolution passes.
- Expose `gaussian_blur(image, sigma)` to Python.
- Verify constant images, impulse responses, and normalization.

Deliverable: an efficient filter built from reusable primitives.

### Step 7: add Sobel gradients

- Calculate horizontal and vertical Sobel responses.
- Calculate gradient magnitude using floating-point intermediates.
- Define and document output conversion behavior.
- Verify orientation and magnitude on synthetic edges.

Deliverable: a composable edge-detection building block.

### Step 8: add resizing

- Implement nearest-neighbor coordinate mapping.
- Implement bilinear sampling and interpolation.
- Support grayscale and RGB inputs.
- Test identity, enlargement, reduction, corners, and one-pixel dimensions.

Deliverable: two geometric resampling methods sharing a common sampling layer.

### Step 9: harden and document version 0.1

- Run the C++ and Python test suites.
- Run compiler warnings and sanitizers.
- Add representative benchmarks.
- Add a complete Python usage example.
- Test installation in a fresh virtual environment.
- Document supported shapes, datatypes, borders, and output behavior.

Deliverable: a tested, installable, documented version 0.1 package.

## Testing strategy

Every operation should include:

- A small hand-computed correctness case
- Empty or invalid input checks where applicable
- Shape and datatype validation tests
- Border and corner cases
- Grayscale and RGB coverage when supported
- A Python integration test
- Comparison with a simple independent reference implementation

Tests should compare exact values for integer operations and use explicit tolerances for floating-point calculations.

## Definition of done for version 0.1

Version 0.1 is complete when:

- The package installs into a clean Python environment.
- All planned operations are callable with documented NumPy inputs.
- The C++ core builds without Python headers.
- C++ unit tests and Python integration tests pass.
- Invalid inputs produce clear exceptions rather than crashes.
- Sanitizer checks report no memory errors in the test suite.
- At least one end-to-end example loads, processes, and saves an image.
- The process for adding an operation is documented and demonstrated.

## Adding future techniques

Each new technique should follow this checklist:

1. Choose the appropriate operation module or create a cohesive new one.
2. Add a public C++ declaration under `include/lumen/ops/`.
3. Implement it under `src/ops/` using existing primitives where possible.
4. Add focused C++ correctness tests.
5. Add a thin binding in the matching Python binding file.
6. Re-export and document the Python function.
7. Add Python validation and reference tests.
8. Benchmark it if performance is part of its purpose.

Likely post-0.1 modules include morphology, histograms, segmentation, geometric transforms, feature extraction, and optimized execution backends.

## Deferred work

The following are deliberately deferred until the scalar CPU implementation is correct and stable:

- Non-contiguous NumPy input
- Additional numeric datatypes
- Arbitrary channel layouts
- In-place Python operations
- SIMD dispatch
- Multithreading
- GPU acceleration
- Image file codecs
- Complex pipeline or graph APIs
- Platform wheel publishing

Deferring these features keeps the initial implementation understandable while leaving explicit extension points for them.
