#include <iostream>
#include "factories/encoder_factory.h"
#include "common/barcode_types.h"
#include "common/barcode_options.h"
using namespace std;

int main() {
    try {
        BarcodeOptions options;

        // Тест 1 — создание Aztec энкодера
        auto encoder = EncoderFactory::Create(BarcodeType::Aztec);
        if (!encoder) {
            throw runtime_error("Aztec encoder is null!");
        }
        cout << "[OK] Aztec encoder created" << endl;

        // Тест 2 — генерация через энкодер
        auto matrix = encoder->Encode("HELLO_FACTORY", options);
        if (matrix.IsEmpty()) {
            throw runtime_error("Matrix is empty!");
        }
        cout << "[OK] Matrix generated via factory" << endl;

        // Тест 3 — создание QR энкодера
        auto qrEncoder = EncoderFactory::Create(BarcodeType::QR);
        if (!qrEncoder) {
            throw runtime_error("QR encoder is null!");
        }
        cout << "[OK] QR encoder created" << endl;

        // Тест 4 — создание DataMatrix энкодера
        auto dmEncoder = EncoderFactory::Create(BarcodeType::DataMatrix);
        if (!dmEncoder) {
            throw runtime_error("DataMatrix encoder is null!");
        }
        cout << "[OK] DataMatrix encoder created" << endl;

        cout << "[SUCCESS] All factory tests passed!" << endl;

    }
    catch (const exception& e) {
        cerr << "[FAIL] " << e.what() << endl;
        return 1;
    }
    return 0;
}