#include "datamatrix_encoder.h"
#include "BarcodeFormat.h"
#include "MultiFormatWriter.h"

#include <stdexcept>

using namespace std;

ZXing::BitMatrix DataMatrixEncoder::encode(const wstring& text, int width, int height)
{
    if (text.empty()) {
        throw invalid_argument("Input text must not be empty");
    }

    if (width <= 0 || height <= 0) {
        throw invalid_argument("Width and height must be positive");
    }

    try {
        ZXing::MultiFormatWriter writer(ZXing::BarcodeFormat::DataMatrix);
        return writer.encode(text, width, height);
    }
    catch (const exception& e) {
        throw runtime_error("DataMatrix encoding failed: " + string(e.what()));
    }
}

ZXing::BitMatrix DataMatrixEncoder::encode(const wstring& text)
{
    // default size
    return encode(text, 100, 100);
}