#pragma once

#include <string>

#include "common/barcode_options.h"
#include "common/barcode_types.h"
#include "renderers/svg_renderer.h"

class BarcodeService {
public:
    BarcodeService() = default;

    [[nodiscard]] std::string Generate(
        const std::string& data,
        BarcodeType type,
        const BarcodeOptions& options
    ) const;

private:
    SvgRenderer renderer_;
};