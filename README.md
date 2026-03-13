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

- `docs/` — project documentation, architecture, UML diagrams
- `src/` — project source code
- `tests/` — unit tests
- `Dockerfile` — optional reproducible build environment

## Architecture Overview

The system consists of the following main modules:

- `BarcodeEncoder` — barcode generation using ZXing-cpp
- `SvgRenderer` — conversion of barcode matrix into SVG
- `BarcodeService` — main service for barcode generation
- `ComponentInterface` — integration with 1C external component API

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
