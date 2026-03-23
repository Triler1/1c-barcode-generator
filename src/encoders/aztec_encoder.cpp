#include "AztecEncoder.h"
#include <BarcodeFormat.h>
#include <MultiFormatWriter.h>

ZXing::BitMatrix AztecEncoder::Encode(const std::string& data) {
    auto writer = ZXing::MultiFormatWriter(ZXing::BarcodeFormat::Aztec);
    auto matrix = writer.encode(data, 200, 200);
    return matrix;
}