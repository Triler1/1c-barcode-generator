# Project Documentation

This directory contains the project documentation for the SVG Barcode Generator.

## Contents

- [System Architecture](architecture.md) — description of the system architecture and main modules  
- [Implementation Plan](implementation-plan.md) — planned steps for implementing the project  
- UML Class Diagram (`uml-class-diagram.png`)

## Overview

The project implements a C++ external component for **1C:Enterprise** that generates 2D barcodes (QR, DataMatrix, Aztec) in **SVG format**.

The system uses the **ZXing-cpp** library to encode barcode data and converts the resulting matrix into an SVG image.

## Main Modules

The system consists of the following modules:

- **IEncoder** — common interface defining the contract for barcode encoders  
- **QREncoder** — encoder implementation for QR codes  
- **DataMatrixEncoder** — encoder implementation for DataMatrix codes  
- **AztecEncoder** — encoder implementation for Aztec codes  
- **SvgRenderer** — converts barcode matrices into SVG images  
- **BarcodeService** — provides the public API for barcode generation  
- **ComponentInterface** — integration with the 1C external component API  

## UML Class Diagram

![UML Class Diagram](uml-class-diagram.png)
