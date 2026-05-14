#include "encoders/aztec_encoder.h"
#include "CreateBarcode.h"
#include "BitMatrix.h"
#include <stdexcept>

BarcodeMatrix AztecEncoder::Encode(const std::string& data, const BarcodeOptions& options) const {
    if (data.empty()) {
        throw std::invalid_argument("Data must not be empty");
    }

    auto creatorOpts = ZXing::CreatorOptions(ZXing::BarcodeFormat::AztecCode);
    auto barcode = ZXing::CreateBarcodeFromText(data, creatorOpts);

    auto imageView = barcode.symbol();
    int w = imageView.width();
    int h = imageView.height();

    BarcodeMatrix result(w, h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            result.Set(x, y, imageView.data()[y * imageView.rowStride() + x] == 0);
        }
    }
    return result;
}