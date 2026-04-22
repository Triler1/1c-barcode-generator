#pragma once

#include "encoders/iencoder.h"

#include <ZXing/QRCode/QRErrorCorrectionLevel.h>
#include <string>

class QREncoder : public IEncoder {
public:
    explicit QREncoder(
        ZXing::QRCode::ErrorCorrectionLevel ecLevel =
            ZXing::QRCode::ErrorCorrectionLevel::Medium,
        int version = 0);

    ~QREncoder() override = default;

    BarcodeMatrix Encode(const std::string& data, const BarcodeOptions& options) const override;

private:
    ZXing::QRCode::ErrorCorrectionLevel ecLevel_;
    int version_;
};
