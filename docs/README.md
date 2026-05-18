# Project Documentation

Welcome to the documentation section of **SVG Barcode Generator for 1C**.

This directory contains architecture notes, implementation details, diagrams, and development plans for the project.

The main repository README provides a general overview, build instructions, and project status.  
This documentation section focuses on the internal design and implementation details.

---

## Documentation Map

| Document | Description |
|---|---|
| [Architecture](architecture.md) | System architecture, module responsibilities, and component interaction |
| [Implementation Plan](implementation-plan.md) | Development plan, implementation stages, and planned improvements |
| [UML Class Diagram](uml-class-diagram.png) | Visual class diagram of the core project structure |
| [PlantUML Source](uml-class-diagram.puml) | Source file for generating the UML class diagram |

---

## Project Overview

**SVG Barcode Generator for 1C** is a native C++ external component for **1C:Enterprise**.

It generates 2D barcodes and returns them as **SVG markup**.

Supported barcode types:

| Barcode type | Output |
|---|---|
| QR Code | SVG |
| DataMatrix | SVG |
| Aztec | SVG |

The project uses **ZXing-cpp** for barcode encoding and converts the generated barcode matrix into an SVG image.

---

## Architecture Summary

The project is split into independent layers:

```text
1C:Enterprise
      |
      v
ComponentInterface
      |
      v
BarcodeService
      |
      v
EncoderFactory
      |
      v
IEncoder
  |       |        |
  v       v        v
QREncoder DataMatrixEncoder AztecEncoder
      |
      v
BarcodeMatrix
      |
      v
SvgRenderer
      |
      v
SVG output
```

---

## Main Modules

| Module | Responsibility |
|---|---|
| `IEncoder` | Common interface for barcode encoders |
| `QREncoder` | QR Code encoder implementation |
| `DataMatrixEncoder` | DataMatrix encoder implementation |
| `AztecEncoder` | Aztec encoder implementation |
| `EncoderFactory` | Creates an encoder based on the requested barcode type |
| `BarcodeService` | Coordinates barcode generation and SVG rendering |
| `SvgRenderer` | Converts barcode matrices into SVG markup |
| `BarcodeMatrix` | Stores the generated barcode module matrix |
| `BarcodeOptions` | Stores rendering options such as module size and margin |
| `ComponentInterface` | Connects the C++ core with the 1C external component API |

---

## Core Generation Flow

The typical barcode generation flow looks like this:

1. 1C calls a method exposed by `ComponentInterface`.
2. `ComponentInterface` extracts input data and options from 1C parameters.
3. `BarcodeService` receives the request.
4. `EncoderFactory` creates the required encoder.
5. The selected encoder generates a `BarcodeMatrix`.
6. `SvgRenderer` converts the matrix into SVG markup.
7. The SVG string is returned back to 1C.

---

## UML Class Diagram

<p align="center">
  <img src="uml-class-diagram.png" alt="UML Class Diagram" width="760">
</p>

<p align="center">
  <i>Core class structure</i>
</p>

The diagram source is available here:

[uml-class-diagram.puml](uml-class-diagram.puml)

---

## Directory Structure

```text
docs/
├── README.md
├── architecture.md
├── implementation-plan.md
├── uml-class-diagram.png
└── uml-class-diagram.puml
```

---

## Recommended Reading Order

For a quick understanding of the project, read the documents in this order:

1. [Project README](../README.md)
2. [Architecture](architecture.md)
3. [UML Class Diagram](uml-class-diagram.png)
4. [Implementation Plan](implementation-plan.md)

---

## Development Notes

When updating the project documentation:

- keep diagrams synchronized with the source code;
- update `architecture.md` when module responsibilities change;
- update `implementation-plan.md` when tasks are completed or priorities change;
- regenerate `uml-class-diagram.png` after changing `uml-class-diagram.puml`;
- keep this file as the entry point for all documentation.

---

## Planned Documentation Improvements

Possible future documentation additions:

- 1C usage examples;
- external component registration guide;
- platform-specific deployment notes;
- generated SVG examples;
- troubleshooting guide;
- testing guide;
- release packaging guide.
