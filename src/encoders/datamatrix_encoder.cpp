#include "encoders/datamatrix_encoder.h"
#include <ZXing/BarcodeFormat.h>
#include <ZXing/MultiFormatWriter.h>
#include <ZXing/BitMatrix.h>

#include <stdexcept>
#include <string>

BarcodeMatrix DataMatrixEncoder::Encode(const std::string& data, const BarcodeOptions& options) const {
    try {
        ZXing::MultiFormatWriter writer(ZXing::BarcodeFormat::DataMatrix);

        auto zxMatrix = writer.encode(data, 0, 0);
        
        return result; 
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Ошибка DataMatrix: " + std::string(e.what()));
    }
}