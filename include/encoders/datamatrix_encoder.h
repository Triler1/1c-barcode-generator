#pragma once
#include <string>
#include "BitMatrix.h" 

class DataMatrixEncoder {
public:
    static ZXing::BitMatrix encode(const std::wstring& text, int width, int height);
};