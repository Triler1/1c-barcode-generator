#include "datamatrix_encoder.h"
#include "BarcodeFormat.h"  
#include "MultiFormatWriter.h" 
#include <stdexcept>
using namespace std;
ZXing::BitMatrix DataMatrixEncoder::encode(const wstring& text, int width, int height) {
    try {
        ZXing::MultiFormatWriter writer(ZXing::BarcodeFormat::DataMatrix);

        return writer.encode(text, width, height);
    }
    catch (const exception& e) {
        throw runtime_error("Error DataMatrix: " + string(e.what()));
    }
}