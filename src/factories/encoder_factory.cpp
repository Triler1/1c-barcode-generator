#include "encoder_factory.h"
#include "aztec_encoder.h"
#include "qr_encoder.h"
#include "datamatrix_encoder.h"
#include "common/barcode_types.h"
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