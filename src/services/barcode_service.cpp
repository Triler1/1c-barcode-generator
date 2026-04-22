#include "services/barcode_service.h"

#include <memory>
#include <stdexcept>

#include "encoders/iencoder.h"
#include "factories/encoder_factory.h"

std::string BarcodeService::Generate(const std::string& data, BarcodeType type, const BarcodeOptions& options) const {
    std::unique_ptr<IEncoder> encoder = EncoderFactory::Create(type);
    if (!encoder) {
        throw std::runtime_error("Failed to create encoder");
    }
    BarcodeMatrix matrix = encoder->Encode(data, options);
    return renderer_.Render(matrix, options);
}