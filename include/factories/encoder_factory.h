#pragma once

#include <memory>

#include "common/barcode_types.h"
#include "encoders/iencoder.h"

class EncoderFactory {
public:
    [[nodiscard]] static std::unique_ptr<IEncoder> Create(BarcodeType type);
};