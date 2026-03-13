# SVG Barcode Generator for 1C

C++ external component for **1C:Enterprise** that generates **2D barcodes (QR, DataMatrix, Aztec, etc.) in SVG format**.

---

# Project Goal

The goal of this project is to develop an external component for **1C:Enterprise** that allows generating 2D barcodes in **SVG format**.

The component uses the **ZXing-cpp** library to encode barcode data and converts the result into SVG images.

---

# Supported Barcode Types (Planned)

The component is planned to support the following barcode types:

- QR Code
- DataMatrix
- Aztec

---

# Repository Structure

├── README.md  
├── docs/        # documentation and UML diagrams
├── src/         # source code
└── tests/       # unit tests

---

# Implementation Plan

The system will consist of several modules:

**Barcode Encoder**  
Uses the ZXing-cpp library to encode input data into barcode matrices.

**SVG Renderer**  
Converts the barcode matrix into an SVG image.

**1C Component Interface**  
Provides interaction between the C++ component and 1C:Enterprise.

---

# Technologies

- C++
- ZXing-cpp
- 1C:Enterprise External Component Technology
- SVG

---

# Documentation

Detailed documentation and UML diagrams are available here:

➡ **[Project Documentation](docs/README.md)**

---

# External Libraries

- ZXing-cpp  
https://github.com/zxing-cpp/zxing-cpp

---

# License

This project is created for educational purposes.
