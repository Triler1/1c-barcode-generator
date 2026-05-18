#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "encoders/qr_encoder.h"
#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

class QREncoderTest : public ::testing::Test {
protected:
    QREncoder encoder;
    BarcodeOptions defaults; // ModuleSize=10, Margin=4
};

// 1. Пустая строка → исключение
TEST_F(QREncoderTest, EmptyData_ThrowsInvalidArgument) {
    EXPECT_THROW(
        {
            (void)encoder.Encode("", defaults);
        },
        std::invalid_argument
    );
}

// 2. Валидные данные → матрица не пустая
TEST_F(QREncoderTest, ValidData_ReturnsNonEmptyMatrix) {
    BarcodeMatrix result = encoder.Encode("Hello", defaults);

    EXPECT_FALSE(result.IsEmpty());
    EXPECT_GT(result.GetWidth(), 0u);
    EXPECT_GT(result.GetHeight(), 0u);
}

// 3. Матрица квадратная, QR всегда квадратный
TEST_F(QREncoderTest, ValidData_MatrixIsSquare) {
    BarcodeMatrix result = encoder.Encode("Hello", defaults);

    EXPECT_EQ(result.GetWidth(), result.GetHeight());
}

// 4. Разные данные → разные матрицы
TEST_F(QREncoderTest, DifferentData_ProduceDifferentMatrices) {
    BarcodeMatrix result1 = encoder.Encode("Hello", defaults);
    BarcodeMatrix result2 = encoder.Encode("World", defaults);

    ASSERT_EQ(result1.GetWidth(), result2.GetWidth());
    ASSERT_EQ(result1.GetHeight(), result2.GetHeight());

    bool differ = false;

    for (std::size_t y = 0; y < result1.GetHeight() && !differ; ++y) {
        for (std::size_t x = 0; x < result1.GetWidth() && !differ; ++x) {
            if (result1.Get(x, y) != result2.Get(x, y)) {
                differ = true;
            }
        }
    }

    EXPECT_TRUE(differ);
}

// 5. Одинаковые данные → одинаковые матрицы, детерминированность
TEST_F(QREncoderTest, SameData_ProduceSameMatrix) {
    BarcodeMatrix result1 = encoder.Encode("Hello", defaults);
    BarcodeMatrix result2 = encoder.Encode("Hello", defaults);

    ASSERT_EQ(result1.GetWidth(), result2.GetWidth());
    ASSERT_EQ(result1.GetHeight(), result2.GetHeight());

    for (std::size_t y = 0; y < result1.GetHeight(); ++y) {
        for (std::size_t x = 0; x < result1.GetWidth(); ++x) {
            EXPECT_EQ(result1.Get(x, y), result2.Get(x, y));
        }
    }
}

// 6. Длинная строка → матрица не меньше, чем для короткой
TEST_F(QREncoderTest, LongerData_ProducesLargerOrEqualMatrix) {
    BarcodeMatrix shortResult = encoder.Encode("Hi", defaults);
    BarcodeMatrix longResult = encoder.Encode(std::string(200, 'A'), defaults);

    EXPECT_GE(longResult.GetWidth(), shortResult.GetWidth());
    EXPECT_GE(longResult.GetHeight(), shortResult.GetHeight());
}