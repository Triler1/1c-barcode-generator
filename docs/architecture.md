# System Architecture

The system is designed to generate 2D barcodes (QR, DataMatrix, Aztec) and return them as SVG images.

The architecture is divided into several modules, each responsible for a specific part of the system.

## Modules

### BarcodeEncoder
Responsible for generating barcode matrices using the **ZXing-cpp** library.  
This module encodes the input data into the internal representation of a barcode.

### SvgRenderer
Converts barcode matrices into SVG images.  
It takes the generated matrix and renders it as a scalable vector graphic.

### BarcodeService
Provides the public API for barcode generation.  
This module coordinates the process of generating barcodes by calling the encoder and renderer.

Available methods:
- `GenerateQR`
- `GenerateDataMatrix`
- `GenerateAztec`

### ComponentInterface
Provides integration between the C++ implementation and the **1C external component API**.  
This module allows the barcode generation functionality to be used inside 1C.

## UML Class Diagram

![UML Class Diagram](uml-class-diagram.png)
