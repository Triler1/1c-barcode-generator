#pragma once

#include "encoders/iencoder.h"

#include <string>

class QREncoder : public IEncoder {
public:
    [[nodiscard]] BarcodeMatrix Encode(const std::string& data, const BarcodeOptions& options) const override;
};
