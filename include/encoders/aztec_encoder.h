#pragma once
#include"IEncoder.h"
#include "common/barcode_matrix.h"
// Энкодер для генерации штрихкодов формата Aztec
class AztecEncoder : public IEncoder {
public:
	BarcodeMatrix Encode(const std::string& data, const BarcodeOptions& options) const override;
};