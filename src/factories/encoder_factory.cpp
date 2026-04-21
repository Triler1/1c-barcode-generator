#include "factories/encoder_factory.h"
#include "encoders/aztec_encoder.h"
#include "encoders/qr_encoder.h"
#include "encoders/datamatrix_encoder.h"
#include <stdexcept>

IEncoder* EncoderFactory::Create(const BarcodeType& type) {
    switch (type) {
    case BarcodeType::Aztec:
        return new AztecEncoder();
    case BarcodeType::QR:
        return new QREncoder();
    case BarcodeType::DataMatrix:
        return new DataMatrixEncoder();
    default:
        throw std::invalid_argument("Unknown encoder type");
    }
}