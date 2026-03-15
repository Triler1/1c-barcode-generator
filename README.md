# SVG Barcode Generator for 1C

C++ external component for 1C:Enterprise for generating 2D barcodes (QR, DataMatrix, Aztec, etc.) in SVG format.

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

- `include/` — public headers and encoder interfaces
- `src/` — implementation of encoders and services
- `docs/` — project documentation and UML diagrams
- `tests/` — unit tests

## Architecture Overview

The system is built using a modular architecture with separate encoders for different barcode types.

Main components of the system:

- `IEncoder` — common interface defining the contract for barcode encoders
- `QREncoder` — encoder implementation for QR codes
- `DataMatrixEncoder` — encoder implementation for DataMatrix codes
- `AztecEncoder` — encoder implementation for Aztec codes
- `BarcodeService` — public API used to generate barcodes and return SVG output
- `SvgRenderer` — converts barcode matrices into SVG images
- `ComponentInterface` — integration layer between the C++ code and the 1C external component API

See the UML diagram in the documentation:

[UML Class Diagram](docs/uml-class-diagram.png)

## Build System

The project uses **CMake** as the build system.

## Documentation

Detailed project documentation, implementation plan, and UML diagrams are available here:

[Project Documentation](docs/README.md)

## External Libraries

- **ZXing-cpp** — library used for generating barcode matrices  
  https://github.com/zxing-cpp/zxing-cpp

## License

This project is created for educational purposes.
