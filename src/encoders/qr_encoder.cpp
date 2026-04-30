#include "encoders/qr_encoder.h"
#include <ZXing/ZXingCpp.h>
#include <stdexcept>

BarcodeMatrix QREncoder::Encode(const std::string& data, const BarcodeOptions& options) const {
    if (data.empty()) {
        throw std::invalid_argument("Data must not be empty");
    }

    // 1. Создаём QR-код через ZXing
    auto barcode = ZXing::CreateBarcodeFromText(
        data,
        ZXing::CreatorOptions(ZXing::BarcodeFormat::QRCode)
    );

    // 2. Рендерим в изображение
    auto image = ZXing::WriteBarcodeToImage(
        barcode,
        ZXing::WriterOptions().scale(1).addQuietZones(false)
    );

    // 3. Извлекаем сырые данные изображения
    const int w = image.width();
    const int h = image.height();
    const int pixStride = image.pixStride();
    const int rowStride = image.rowStride();
    const uint8_t* base = image.data();   

    // 4. Копируем в наш внутренний BarcodeMatrix
    BarcodeMatrix result(w, h);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // тёмный пиксель (0) = закрашенный модуль QR = true
            result.Set(x, y, base[y * rowStride + x * pixStride] == 0);
        }
    }

    // 5. Возвращаем наш тип — ZXing остаётся скрытым внутри .cpp
    return result;
}
