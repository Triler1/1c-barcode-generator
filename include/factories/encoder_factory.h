#pragma once
#include "common/barcode_types.h"
#include "encoders/IEncoder.h"

class EncoderFactory {
public:
    static IEncoder* Create(const BarcodeType& type);
};