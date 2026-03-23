#pragma once
#include"IEncoder.h"
#include <BitMatrix.h>

class AztecEncoder : public IEncoder {
public:
	ZXing::BitMatrix Encode(const std::string& data) override;
};