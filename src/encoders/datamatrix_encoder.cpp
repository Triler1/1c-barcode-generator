#include "encoders/datamatrix_encoder.h"
#include <BarcodeFormat.h>
#include <MultiFormatWriter.h>
#include <BitMatrix.h>
#include <stdexcept>

BarcodeMatrix DataMatrixEncoder::Encode(const std::string& data, const BarcodeOptions& options) const {
    if (data.empty()) {
        throw std::invalid_argument("Data must not be empty");
    }
    const int BarcodeSize = 200;
    try {
        auto writer = ZXing::MultiFormatWriter(ZXing::BarcodeFormat::DataMatrix);
        auto zxMatrix = writer.encode(data, BarcodeSize, BarcodeSize);
        BarcodeMatrix result(zxMatrix.width(), zxMatrix.height());

        for (int y = 0; y < zxMatrix.height(); y++) {
            for (int x = 0; x < zxMatrix.width(); x++) {
                result.Set(x, y, zxMatrix.get(x, y));
            }
        }
        return result;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Ошибка DataMatrix: " + std::string(e.what()));
    }
}