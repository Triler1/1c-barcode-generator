#pragma once

#include "encoders/iencoder.h"
#include "common/barcode_matrix.h"
#include "common/barcode_options.h"
#include <string>

class DataMatrixEncoder : public IEncoder {
public:

    [[nodiscard]] BarcodeMatrix Encode(const std::string& data, const BarcodeOptions& options) const override;
};