#include "AztecEncoder.h"
#include <BarcodeFormat.h>
#include <MultiFormatWriter.h>

ZXing::BitMatrix AztecEncoder::Encode(const std::string& data) {
    const int BarcodeSize = 200;
    auto writer = ZXing::MultiFormatWriter(ZXing::BarcodeFormat::Aztec);
    auto matrix = writer.encode(data, BarcodeSize, BarcodeSize);
    return matrix;
}