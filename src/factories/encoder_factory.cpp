#include "factories/encoder_factory.h"

#include "encoders/aztec_encoder.h"
#include "encoders/qr_encoder.h"
#include "encoders/datamatrix_encoder.h"

#include <stdexcept>
#include <memory>

std::unique_ptr<IEncoder> EncoderFactory::Create(BarcodeType type) {
    switch (type) {
        case BarcodeType::Aztec:
            return std::make_unique<AztecEncoder>();
        case BarcodeType::QR:
            return std::make_unique<QREncoder>();
        case BarcodeType::DataMatrix:
            return std::make_unique<DataMatrixEncoder>();
        default:
            throw std::invalid_argument("Unsupported barcode type");
    }
}