#include "encoders/qr_encoder.h"
#include "CreateBarcode.h"
#include <stdexcept>

BarcodeMatrix QREncoder::Encode(const std::string& data, const BarcodeOptions& options) const {
    if (data.empty()) {
        throw std::invalid_argument("Data must not be empty");
    }
    if (options.ModuleSize <= 0) {
        throw std::invalid_argument("ModuleSize must be positive");
    }
    if (options.Margin < 0) {
        throw std::invalid_argument("Margin must be non-negative");
    }
    try {
        auto creatorOpts = ZXing::CreatorOptions(ZXing::BarcodeFormat::QRCode);
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
    catch (const std::exception& e) {
        throw std::runtime_error("Ошибка QR: " + std::string(e.what()));
    }
}