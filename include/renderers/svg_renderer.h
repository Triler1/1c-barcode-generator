#pragma once

#include <string>

#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

class SvgRenderer {
public:
    [[nodiscard]] std::string Render(const BarcodeMatrix& matrix, const BarcodeOptions& options) const;
};