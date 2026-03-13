# Project Documentation

This directory contains the project documentation for the SVG Barcode Generator.

## Contents

- **architecture.md** — description of the system architecture and main modules  
- **implementation-plan.md** — planned steps for implementing the project  
- **uml-diagram.png** — UML diagram of the system structure

## Overview

The project implements a C++ external component for **1C:Enterprise** that generates 2D barcodes (QR, DataMatrix, Aztec) in **SVG format**.

The system uses the **ZXing-cpp** library to encode barcode data and converts the resulting matrix into an SVG image.

## Main Modules

The system consists of the following modules:

- **BarcodeEncoder** — generates barcode matrices using ZXing-cpp  
- **SvgRenderer** — converts barcode matrices into SVG images  
- **BarcodeService** — provides public API for barcode generation  
- **ComponentInterface** — integration with 1C external component API
