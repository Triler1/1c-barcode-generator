#pragma once

#include <string>

#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

class IEncoder {
public:
	virtual BarcodeMatrix Encode(const std::string& data, const BarcodeOptions& options) const = 0;
	virtual ~IEncoder() = default;
};