#pragma once
#include<string>
#include <BitMatrix.h>

class IEncoder {
public:
	virtual ZXing::BitMatrix Encode(const std::string& data) = 0;
	virtual ~IEncoder() = default;
};