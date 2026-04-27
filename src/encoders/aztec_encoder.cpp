#include "encoders/aztec_encoder.h"
#include <ZXing/BarcodeFormat.h>
#include <ZXing/MultiFormatWriter.h>
#include <ZXing/BitMatrix.h>
#include <stdexcept>

BarcodeMatrix AztecEncoder::Encode(const std::string& data, const BarcodeOptions& options) const {
    if (data.empty()) {
        throw std::invalid_argument("Data must not be empty");
    }
    const int BarcodeSize = 200;
    auto writer = ZXing::MultiFormatWriter(ZXing::BarcodeFormat::Aztec);
    auto zxMatrix = writer.encode(data, BarcodeSize, BarcodeSize);
    BarcodeMatrix result(zxMatrix.width(), zxMatrix.height());
    for (int y = 0; y < zxMatrix.height(); y++) {
        for (int x = 0; x < zxMatrix.width(); x++) {
            result.Set(x, y, zxMatrix.get(x, y));
        }
    }
    return result;
}