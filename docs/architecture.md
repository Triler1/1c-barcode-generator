# System Architecture

This document describes the internal architecture of **SVG Barcode Generator for 1C**.

The project is designed to generate 2D barcodes and return them as **SVG markup**.  
It uses **ZXing-cpp** for barcode encoding and a custom SVG renderer for producing scalable vector output.

---

## High-Level Overview

The system follows a modular layered architecture.

Each layer has a clear responsibility:

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

The main idea is to keep barcode encoding, SVG rendering, business logic, and 1C integration separated from each other.

---

## Supported Barcode Types

| Barcode type | Encoder |
|---|---|
| QR Code | `QREncoder` |
| DataMatrix | `DataMatrixEncoder` |
| Aztec | `AztecEncoder` |

All encoders implement the common `IEncoder` interface.

---

## Core Modules

### `ComponentInterface`

`ComponentInterface` is the integration layer between the C++ implementation and the **1C external component API**.

It is responsible for:

- exposing methods to 1C;
- reading parameters passed from 1C;
- selecting the requested barcode type;
- preparing `BarcodeOptions`;
- calling `BarcodeService`;
- returning the generated SVG string back to 1C.

From the 1C side, the component exposes barcode generation methods such as:

- `GenerateQR`
- `GenerateDataMatrix`
- `GenerateAztec`

The component also provides localized method aliases where applicable.

---

### `BarcodeService`

`BarcodeService` is the main service layer of the application.

It coordinates the complete barcode generation process:

1. receives input data, barcode type, and rendering options;
2. requests the required encoder from `EncoderFactory`;
3. generates a `BarcodeMatrix`;
4. passes the matrix to `SvgRenderer`;
5. returns SVG markup as a string.

This class keeps the public generation workflow simple and hides the details of encoder selection and rendering.

---

### `EncoderFactory`

`EncoderFactory` is responsible for creating barcode encoders.

It receives a `BarcodeType` value and returns an encoder implementing the `IEncoder` interface.

This allows `BarcodeService` to work with all barcode types through a single abstraction.

---

### `IEncoder`

`IEncoder` defines the common interface for barcode encoders.

Each encoder receives:

- input data;
- barcode options.

Each encoder returns a `BarcodeMatrix`.

This interface allows QR, DataMatrix, and Aztec encoders to be used interchangeably by the service layer.

---

### `QREncoder`

`QREncoder` implements QR Code generation using **ZXing-cpp**.

It converts input text into a QR barcode matrix and returns it as a `BarcodeMatrix`.

---

### `DataMatrixEncoder`

`DataMatrixEncoder` implements DataMatrix barcode generation using **ZXing-cpp**.

It converts input text into a DataMatrix barcode matrix and returns it as a `BarcodeMatrix`.

---

### `AztecEncoder`

`AztecEncoder` implements Aztec barcode generation using **ZXing-cpp**.

It converts input text into an Aztec barcode matrix and returns it as a `BarcodeMatrix`.

---

### `BarcodeMatrix`

`BarcodeMatrix` is an internal data structure used to store the generated barcode modules.

It stores:

- matrix width;
- matrix height;
- module values, where each cell represents a black or white barcode module.

The renderer uses this matrix to generate SVG rectangles.

---

### `BarcodeOptions`

`BarcodeOptions` stores rendering options used during SVG generation.

Typical options include:

| Option | Description |
|---|---|
| `ModuleSize` | Size of a single barcode module in SVG units |
| `Margin` | Quiet zone around the barcode |

These options affect the final SVG dimensions and visual layout.

---

### `SvgRenderer`

`SvgRenderer` converts a `BarcodeMatrix` into SVG markup.

It is responsible for:

- calculating SVG dimensions;
- adding a white background;
- rendering black barcode modules as SVG rectangles;
- applying module size and margin options.

The renderer does not know which barcode type was used.  
It only works with the generated matrix.

---

## Generation Flow

The typical barcode generation flow is:

1. 1C calls one of the exposed component methods.
2. `ComponentInterface` extracts input parameters.
3. `ComponentInterface` selects the requested `BarcodeType`.
4. `ComponentInterface` creates `BarcodeOptions`.
5. `BarcodeService` receives the generation request.
6. `EncoderFactory` creates the required encoder.
7. The selected encoder generates a `BarcodeMatrix`.
8. `SvgRenderer` converts the matrix into SVG.
9. The SVG string is returned back to 1C.

In short:

```text
1C
  -> ComponentInterface
  -> BarcodeService
  -> EncoderFactory
  -> IEncoder implementation
  -> BarcodeMatrix
  -> SvgRenderer
  -> SVG string
```

---

## Design Principles

The architecture follows several design principles:

### Separation of Concerns

Each module has a single responsibility:

- encoders generate barcode matrices;
- renderer generates SVG;
- service coordinates the workflow;
- component interface handles 1C integration.

### Extensibility

New barcode types can be added by:

1. creating a new encoder class;
2. implementing `IEncoder`;
3. adding a new value to `BarcodeType`;
4. updating `EncoderFactory`;
5. exposing a method through `ComponentInterface` if needed.

### Testability

Core modules are independent from the 1C integration layer.

This makes it possible to test:

- encoders;
- factory logic;
- barcode service;
- SVG rendering;
- matrix behavior.

### Platform Independence

The core generation logic is platform-independent.

Platform-specific details are isolated in the component integration and build configuration.

---

## Error Handling

The system validates common invalid inputs, such as:

- empty input data;
- unsupported barcode type;
- invalid rendering options;
- barcode generation errors returned by ZXing-cpp.

Errors from the encoding layer are propagated to the service/component layer and can be returned to the caller as failure information.

---

## UML Class Diagram

The current class structure is shown in the UML diagram:

![UML Class Diagram](uml-class-diagram.png)

The diagram source is available in:

[uml-class-diagram.puml](uml-class-diagram.puml)

---

## Related Documentation

- [Project Documentation](README.md)
- [Implementation Plan](implementation-plan.md)
- [UML Class Diagram](uml-class-diagram.png)
