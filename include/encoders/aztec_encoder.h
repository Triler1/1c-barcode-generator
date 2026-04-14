#pragma once
#include"IEncoder.h"
#include <BitMatrix.h>
// Энкодер для генерации штрихкодов формата Aztec
class AztecEncoder : public IEncoder {
public:
	ZXing::BitMatrix Encode(const std::string& data) override;
};