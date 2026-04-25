#include "encoders/qr_encoder.h"

#include <ZXing/Barcode.h>
#include <ZXing/BarcodeFormat.h>
#include <ZXing/CreateBarcode.h>

#include <stdexcept>

BarcodeMatrix QREncoder::Encode(const std::string& data, const BarcodeOptions& options) const
{
    
}

