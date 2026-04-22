#include "encoders/qr_encoder.h"

#include <ZXing/BarcodeFormat.h>
#include <ZXing/MultiFormatWriter.h>
#include <ZXing/WriterException.h>
А к
#include <stdexcept>

QREncoder::QREncoder(ZXing::QRCode::ErrorCorrectionLevel ecLevel, int version)
    : ecLevel_(ecLevel), version_(version)
{
    if (version < 0 || version > 40) {
        throw std::invalid_argument(
            "QREncoder: version must be 0 (auto) or 1–40, got " +
            std::to_string(version));
    }
}

ZXing::BitMatrix QREncoder::Encode(const std::string& data)
{
    if (data.empty()) {
        throw std::invalid_argument("QREncoder::Encode – data must not be empty");
    }

    ZXing::MultiFormatWriter writer(ZXing::BarcodeFormat::QRCode);
    writer.setEccLevel(static_cast<int>(ecLevel_));

    if (version_ != 0) {
        writer.setVersion(version_);
    }

    try {
        return writer.encode(data, 0, 0);
    } catch (const ZXing::WriterException& ex) {
        throw std::runtime_error(
            std::string("QREncoder::Encode – ZXing encoding failed: ") + ex.what());
    }
}
