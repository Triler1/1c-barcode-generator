#pragma once
#include <string>
#include <BitMatrix.h>

class SvgRenderer {
public:
    std::string Render(const ZXing::BitMatrix& matrix);
};