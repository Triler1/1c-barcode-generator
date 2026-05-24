#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "encoders/datamatrix_encoder.h"
#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

class DataMatrixEncoderTest : public ::testing::Test {
protected:
    DataMatrixEncoder encoder;
    BarcodeOptions defaults; // ModuleSize=10, Margin=4
};

TEST_F(DataMatrixEncoderTest, Dummy) {
    EXPECT_TRUE(true);
}

// Тест на пустую строку
TEST_F(DataMatrixEncoderTest, EmptyData_ThrowsInvalidArgument) {
    EXPECT_THROW(
        {
            (void)encoder.Encode("", defaults);
        },
        std::invalid_argument
    );
}

// Тест на непустой результат
TEST_F(DataMatrixEncoderTest, ValidData_ReturnsNonEmptyMatrix) {
    BarcodeMatrix result = encoder.Encode("Hello", defaults);
    EXPECT_FALSE(result.IsEmpty());
}

// Квадратность матрицы
TEST_F(DataMatrixEncoderTest, ValidData_MatrixHasPositiveDimensions) {
    BarcodeMatrix result = encoder.Encode("Test", defaults);
    EXPECT_GT(result.GetWidth(), 0u);
    EXPECT_GT(result.GetHeight(), 0u);
}

// Разные данные – разные матрицы
TEST_F(DataMatrixEncoderTest, DifferentData_ProduceDifferentMatrices) {
    BarcodeMatrix result1 = encoder.Encode("Hello", defaults);
    BarcodeMatrix result2 = encoder.Encode("World", defaults);

    bool differ = false;

    ASSERT_EQ(result1.GetWidth(), result2.GetWidth());
    ASSERT_EQ(result1.GetHeight(), result2.GetHeight());

    for (std::size_t y = 0; y < result1.GetHeight() && !differ; ++y) {
        for (std::size_t x = 0; x < result1.GetWidth() && !differ; ++x) {
            if (result1.Get(x, y) != result2.Get(x, y)) {
                differ = true;
            }
        }
    }

    EXPECT_TRUE(differ);
}

// Одинаковые данные – одинаковые матрицы
TEST_F(DataMatrixEncoderTest, SameData_ProduceSameMatrix) {
    BarcodeMatrix result1 = encoder.Encode("Test", defaults);
    BarcodeMatrix result2 = encoder.Encode("Test", defaults);

    ASSERT_EQ(result1.GetWidth(), result2.GetWidth());
    ASSERT_EQ(result1.GetHeight(), result2.GetHeight());

    for (std::size_t y = 0; y < result1.GetHeight(); ++y) {
        for (std::size_t x = 0; x < result1.GetWidth(); ++x) {
            EXPECT_EQ(result1.Get(x, y), result2.Get(x, y));
        }
    }
}

// Длинная строка даёт не меньший размер
TEST_F(DataMatrixEncoderTest, LongerData_ProducesLargerOrEqualMatrix) {
    BarcodeMatrix shortResult = encoder.Encode("Hi", defaults);
    BarcodeMatrix longResult = encoder.Encode(std::string(200, 'A'), defaults);

    EXPECT_GE(longResult.GetWidth(), shortResult.GetWidth());
    EXPECT_GE(longResult.GetHeight(), shortResult.GetHeight());
}

// Спецсимволы и русские буквы не ломают кодировщик
TEST_F(DataMatrixEncoderTest, SpecialCharacters_DoNotCrash) {
    EXPECT_NO_THROW(
        {
            (void)encoder.Encode("Привет\nWorld\t!", defaults);
        }
    );

    BarcodeMatrix result = encoder.Encode("Привет\nWorld\t!", defaults);

    EXPECT_FALSE(result.IsEmpty());
    EXPECT_GT(result.GetWidth(), 0u);
    EXPECT_GT(result.GetHeight(), 0u);
}

// Строка только из цифр успешно кодируется
TEST_F(DataMatrixEncoderTest, DigitsOnly_ReturnsNonEmptyMatrix) {
    BarcodeMatrix result = encoder.Encode("1234567890", defaults);
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_GT(result.GetWidth(), 0u);
    EXPECT_GT(result.GetHeight(), 0u);
}

// Все печатные ASCII‑символы не вызывают падения
TEST_F(DataMatrixEncoderTest, AllPrintableAscii_DoesNotCrash) {
    std::string ascii;
    for (char c = 32; c < 127; ++c) ascii += c;   // 95 символов
    EXPECT_NO_THROW(
        {
            (void)encoder.Encode(ascii, defaults);
        }
    );
    BarcodeMatrix result = encoder.Encode(ascii, defaults);
    EXPECT_FALSE(result.IsEmpty());
}

// В матрице есть хотя бы одна чёрная ячейка
TEST_F(DataMatrixEncoderTest, Matrix_HasAtLeastOneBlackCell) {
    BarcodeMatrix result = encoder.Encode("Some data", defaults);

    bool hasBlack = false;

    for (std::size_t y = 0; y < result.GetHeight(); ++y) {
        for (std::size_t x = 0; x < result.GetWidth(); ++x) {
            if (result.Get(x, y)) {
                hasBlack = true;
            }
        }
    }

    EXPECT_TRUE(hasBlack);
}

// Матрица не полностью чёрная
TEST_F(DataMatrixEncoderTest, Matrix_IsNotCompletelyBlack) {
    BarcodeMatrix result = encoder.Encode("Some data", defaults);

    bool hasWhite = false;

    for (std::size_t y = 0; y < result.GetHeight(); ++y) {
        for (std::size_t x = 0; x < result.GetWidth(); ++x) {
            if (!result.Get(x, y)) {
                hasWhite = true;
            }
        }
    }

    EXPECT_TRUE(hasWhite);
}

// Нулевой ModuleSize → исключение
TEST_F(DataMatrixEncoderTest, ZeroModuleSize_ThrowsInvalidArgument) {
    BarcodeOptions opts;
    opts.ModuleSize = 0;

    EXPECT_THROW(
        {
            (void)encoder.Encode("Test", opts);
        },
        std::invalid_argument
    );
}