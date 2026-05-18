# Implementation Plan

This document describes the implementation plan for **SVG Barcode Generator for 1C**.

The goal of the project is to provide a native C++ external component for **1C:Enterprise** that generates 2D barcodes and returns them as **SVG markup**.

---

## Implementation Goals

The project should provide:

- QR Code generation;
- DataMatrix generation;
- Aztec generation;
- SVG output rendering;
- integration with the 1C external component API;
- unit tests for core modules;
- cross-platform builds for Windows, Linux, and macOS.

---

## Development Stages

### 1. Project Structure

Create the initial project structure:

```text
include/        Public headers and interfaces
src/            Source files and implementation
external/       Third-party dependencies
docs/           Documentation and diagrams
tests/          Unit tests
```

Status: **Done**

---

### 2. ZXing-cpp Integration

Integrate **ZXing-cpp** as the barcode encoding backend.

Tasks:

- add ZXing-cpp as an external dependency;
- configure ZXing-cpp in CMake;
- verify that the project can link against ZXing-cpp;
- use ZXing-cpp for QR, DataMatrix, and Aztec generation.

Status: **Done**

---

### 3. Core Data Structures

Implement common data structures used by the barcode generation pipeline.

Required structures:

| Structure | Purpose |
|---|---|
| `BarcodeType` | Defines supported barcode types |
| `BarcodeOptions` | Stores rendering options such as module size and margin |
| `BarcodeMatrix` | Stores the generated barcode matrix |

Status: **Done**

---

### 4. Encoder Interface

Define the common encoder interface.

The `IEncoder` interface should provide a common contract for all barcode encoders.

Each encoder should:

- accept input data;
- accept barcode options;
- return a `BarcodeMatrix`;
- validate invalid input where applicable.

Status: **Done**

---

### 5. Barcode Encoders

Implement encoder classes for all supported barcode types.

Required encoders:

| Encoder | Barcode type |
|---|---|
| `QREncoder` | QR Code |
| `DataMatrixEncoder` | DataMatrix |
| `AztecEncoder` | Aztec |

Each encoder should use **ZXing-cpp** internally and convert the generated result into the project’s `BarcodeMatrix` format.

Status: **Done**

---

### 6. Encoder Factory

Implement `EncoderFactory`.

Responsibilities:

- receive a `BarcodeType`;
- create the corresponding encoder;
- return the encoder through the `IEncoder` interface;
- handle unsupported barcode types.

Status: **Done**

---

### 7. SVG Renderer

Implement `SvgRenderer`.

Responsibilities:

- convert `BarcodeMatrix` into SVG markup;
- calculate SVG width and height;
- apply module size;
- apply margin;
- render a white background;
- render black modules as SVG rectangles.

Status: **Done**

---

### 8. Barcode Service

Implement `BarcodeService`.

Responsibilities:

- provide a simple public API for barcode generation;
- receive input data, barcode type, and options;
- use `EncoderFactory` to create the required encoder;
- generate a barcode matrix;
- use `SvgRenderer` to produce SVG output;
- return SVG markup as a string.

Status: **Done**

---

### 9. 1C External Component Integration

Implement `ComponentInterface`.

Responsibilities:

- expose component methods to 1C;
- register method names and aliases;
- read parameters from 1C;
- convert 1C values into C++ types;
- call `BarcodeService`;
- return SVG output back to 1C.

Exposed methods:

| Method | Purpose |
|---|---|
| `GenerateQR` | Generate QR Code SVG |
| `GenerateDataMatrix` | Generate DataMatrix SVG |
| `GenerateAztec` | Generate Aztec SVG |

Status: **Done**

---

### 10. Unit Tests

Implement unit tests for core modules.

Test coverage should include:

| Module | Test focus |
|---|---|
| `QREncoder` | input validation, matrix generation, deterministic output |
| `DataMatrixEncoder` | input validation, matrix generation, edge cases |
| `AztecEncoder` | input validation, matrix generation, deterministic output |
| `EncoderFactory` | correct encoder creation |
| `BarcodeService` | SVG generation workflow |
| `SvgRenderer` | SVG dimensions, background, black modules |
| `BarcodeMatrix` | bounds checking and matrix behavior |

Status: **Done**

---

### 11. CMake Test Integration

Connect unit tests to the CMake build system.

Tasks:

- add GoogleTest integration;
- create a test executable;
- register tests with CTest;
- make tests runnable locally and in CI.

Expected commands:

```bash
cmake -B build -S .
cmake --build build
ctest --test-dir build --output-on-failure
```

Status: **Done**

---

### 12. Cross-Platform Build Support

Ensure the component can be built on supported platforms.

Target platforms:

| Platform | Artifact |
|---|---|
| Windows | `BarcodeGenerator.dll` |
| Linux | `libBarcodeGenerator.so` |
| macOS | `libBarcodeGenerator.dylib` |

Tasks:

- configure platform-specific build settings;
- handle Windows-specific entry points;
- support Linux shared library exports;
- support macOS dynamic library generation;
- document platform-specific build commands.

Status: **In Progress**

---

### 13. Continuous Integration

Add GitHub Actions workflow for automated builds.

Tasks:

- build on Linux;
- build on Windows;
- build on macOS;
- install required system dependencies;
- run unit tests with CTest;
- add CI badge to `README.md`.

Status: **In Progress**

---

### 14. Documentation

Maintain project documentation.

Required documentation:

| Document | Purpose |
|---|---|
| `README.md` | Main project overview and build instructions |
| `docs/README.md` | Documentation entry point |
| `docs/architecture.md` | Architecture description |
| `docs/implementation-plan.md` | Development plan |
| `docs/uml-class-diagram.puml` | UML diagram source |
| `docs/uml-class-diagram.png` | Rendered UML diagram |

Status: **In Progress**

---

## Future Improvements

Planned improvements:

- improve Unicode conversion between 1C and C++;
- add real 1C usage examples;
- add deployment instructions for 1C;
- add generated SVG examples;
- improve validation of `BarcodeOptions`;
- add more edge-case tests;
- add release packaging for each supported platform;
- publish prebuilt binaries for Windows, Linux, and macOS;
- add troubleshooting documentation.

---

## Suggested Roadmap

| Stage | Description | Status |
|---|---|---|
| Core architecture | Interfaces, encoders, service, renderer | Done |
| 1C integration | External component API layer | Done |
| Unit testing | GoogleTest-based tests | Done |
| CMake test support | Test executable and CTest integration | Done |
| Cross-platform builds | Windows, Linux, macOS support | In Progress |
| CI | GitHub Actions workflow | In Progress |
| Documentation | README, architecture, UML, examples | In Progress |
| Release packaging | Ready-to-use binary packages | Planned |

---

## Completion Criteria

The project can be considered production-ready when:

- all supported barcode types generate valid SVG;
- the component builds successfully on Windows, Linux, and macOS;
- unit tests pass on all target platforms;
- CI pipeline is stable;
- Unicode handling is reliable;
- 1C usage examples are documented;
- release artifacts are available for supported platforms.
