# System Architecture

The system is designed to generate 2D barcodes (QR, DataMatrix, Aztec) and return them as SVG images.

## Main Modules

### BarcodeEncoder
Responsible for generating barcode matrices using the ZXing-cpp library.

### SvgRenderer
Converts barcode matrices into SVG images.

### BarcodeService
Provides the public API for barcode generation.

Available methods:
- GenerateQR
- GenerateDataMatrix
- GenerateAztec

### ComponentInterface
Provides integration with the 1C external component API.

## Module Interaction

The workflow of the system:

1C → ComponentInterface → BarcodeService → BarcodeEncoder → SvgRenderer → SVG
