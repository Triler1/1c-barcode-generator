
#include "iencoder.h"

#include <ZXing/QRCode/QRErrorCorrectionLevel.h>

#include <string>

class QREncoder : public IEncoder {
public:
    explicit QREncoder(
        ZXing::QRCode::ErrorCorrectionLevel ecLevel =
            ZXing::QRCode::ErrorCorrectionLevel::Medium,
        int version = 0);

    ~QREncoder() override = default;

    ZXing::BitMatrix Encode(const std::string& data) override;

private:
    ZXing::QRCode::ErrorCorrectionLevel ecLevel_;
    int                                  version_;
};
