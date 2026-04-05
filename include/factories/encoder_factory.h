#pragma once
#include <string>
#include "IEncoder.h"
#include "AztecEncoder.h"

class EncoderFactory {
public:
    static IEncoder* Create(const std::string& type);
};