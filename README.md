# SVG Barcode Generator for 1C

![C++20](https://img.shields.io/badge/C%2B%2B-20-0A66C2?logo=c%2B%2B&logoColor=white&style=for-the-badge)
![CMake](https://img.shields.io/badge/CMake-3.20+-1E2A44?logo=cmake&logoColor=white&style=for-the-badge)
![Platforms](https://img.shields.io/badge/Platforms-Windows%20%7C%20Linux%20%7C%20macOS-2D2D2D?logo=windows11&logoColor=white&style=for-the-badge)
![Powered by ZXing](https://img.shields.io/badge/Powered%20by-ZXing--cpp-6B238E?logo=barcode&logoColor=white&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg?style=for-the-badge)

C++ external component for 1C:Enterprise for generating 2D barcodes (QR, DataMatrix, Aztec) in SVG format.

## Project Goal

The goal of this project is to develop an external component for 1C:Enterprise that generates 2D barcodes in SVG format.

The project uses the ZXing-cpp library for barcode encoding and converts the result into SVG.

## Planned Features

- QR Code generation
- DataMatrix generation
- Aztec generation
- SVG output generation
- Integration with 1C external component mechanism
- Unit tests for core modules

## Repository Structure

```
include/        — public headers and encoder interfaces
src/            — implementation of encoders and services
external/       — third-party libraries (ZXing-cpp, 1C Native API SDK)
docs/           — project documentation and UML diagrams
tests/          — unit tests
```

## Architecture Overview

The system is built using a modular architecture with separate encoders for different barcode types.

Main components of the system:

- `IEncoder` — common interface defining the contract for barcode encoders
- `QREncoder` — encoder implementation for QR codes
- `DataMatrixEncoder` — encoder implementation for DataMatrix codes
- `AztecEncoder` — encoder implementation for Aztec codes
- `EncoderFactory` — responsible for creating the appropriate encoder depending on the barcode type
- `BarcodeService` — public API used to generate barcodes and return SVG output
- `SvgRenderer` — converts barcode matrices into SVG images
- `ComponentInterface` — integration layer between the C++ code and the 1C external component API

See the UML diagram in the documentation:

[UML Class Diagram](docs/uml-class-diagram.png)

## Building

### Prerequisites

- CMake 3.20+
- C++20-compatible compiler:
  - **Windows:** MSVC (Visual Studio 2022), MinGW-w64, or Clang
  - **macOS:** Apple Clang (via Xcode Command Line Tools)
  - **Linux:** GCC or Clang

**macOS — install dependencies:**
```bash
xcode-select --install
brew install cmake
```

**Linux — install dependencies:**
```bash
sudo apt install cmake g++ ninja-build
```

### Windows (x86)

1C:Enterprise client on Windows is 32-bit, so the component must be built for x86.

**Visual Studio:**
```bat
cmake -B build -S . -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

**MinGW:**
```bat
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++
cmake --build build
```

The resulting library will be at `build/Release/BarcodeGenerator.dll`.

### macOS (x86_64)

The 1C:Enterprise client for macOS is built for x86_64 and runs via Rosetta on Apple Silicon. Build for x86_64 regardless of your Mac's architecture.

```bash
cmake -B build -S . -DCMAKE_OSX_ARCHITECTURES=x86_64
cmake --build build
```

The resulting library will be at `build/libBarcodeGenerator.dylib`.

### Linux

```bash
cmake -B build -S .
cmake --build build
```

The resulting library will be at `build/libBarcodeGenerator.so`.

## Documentation

Detailed project documentation, implementation plan, and UML diagrams are available here:

[Project Documentation](docs/README.md)

## External Libraries

- **ZXing-cpp** — library used for generating barcode matrices  
  https://github.com/zxing-cpp/zxing-cpp

## License

This project is licensed under the **Apache License 2.0** — see the [LICENSE](LICENSE) file for details.

## Third-Party Licenses

- [zxing-cpp](https://github.com/zxing-cpp/zxing-cpp) — Apache License 2.0 (see [licenses/LICENSE.zxing-cpp](licenses/LICENSE.zxing-cpp)).
