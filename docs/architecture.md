# System Architecture

The system is designed to generate 2D barcodes (QR, DataMatrix, Aztec) and return them as SVG images.

The architecture is modular and separates the encoding logic for different barcode types into independent classes.

## Modules

### IEncoder
Defines the common interface for all barcode encoders.  
Each encoder implementation must provide a method for generating a barcode matrix from input data.

### QREncoder
Implements the encoding logic for QR codes using the **ZXing-cpp** library.

### DataMatrixEncoder
Implements the encoding logic for DataMatrix codes using the **ZXing-cpp** library.

### AztecEncoder
Implements the encoding logic for Aztec codes using the **ZXing-cpp** library.

### SvgRenderer
Responsible for converting barcode matrices into SVG images.

### BarcodeService
Provides the public API for barcode generation.  
It selects the appropriate encoder depending on the requested barcode type and then uses the renderer to produce SVG output.

### ComponentInterface
Provides integration between the C++ implementation and the **1C external component API**, allowing barcode generation to be used inside 1C.

## Module Interaction

The typical workflow of the system:

1C → ComponentInterface → BarcodeService → Encoder → SvgRenderer → SVG

## UML Class Diagram

![UML Class Diagram](uml-class-diagram.png)
