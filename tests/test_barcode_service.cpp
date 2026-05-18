#include <gtest/gtest.h>
#include "services/barcode_service.h"
#include "common/barcode_options.h"
#include "common/barcode_types.h"

static bool Contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

class BarcodeServiceTest : public ::testing::Test {
protected:
    BarcodeService service;
    BarcodeOptions defaults; // ModuleSize=10, Margin=4
};

// 1. Пустая строка → исключение для всех типов
TEST_F(BarcodeServiceTest, EmptyData_QR_ThrowsInvalidArgument) {
    EXPECT_THROW(
    {
        (void)service.Generate("", BarcodeType::QR, defaults);
    },
    std::invalid_argument
    );
}

TEST_F(BarcodeServiceTest, EmptyData_DataMatrix_ThrowsInvalidArgument) {
    EXPECT_THROW(
    {
        (void)service.Generate("", BarcodeType::DataMatrix, defaults);
    },
    std::invalid_argument
    );
}

TEST_F(BarcodeServiceTest, EmptyData_Aztec_ThrowsInvalidArgument) {
    EXPECT_THROW(
    {
        (void)service.Generate("", BarcodeType::Aztec, defaults);
    },
    std::invalid_argument
    );
}

// 2. Валидные данные → возвращается корректный SVG
TEST_F(BarcodeServiceTest, ValidData_QR_ReturnsSvg) {
    std::string result = service.Generate("Hello", BarcodeType::QR, defaults);
    EXPECT_TRUE(Contains(result, "<svg"));
    EXPECT_TRUE(Contains(result, "</svg>"));
}

TEST_F(BarcodeServiceTest, ValidData_DataMatrix_ReturnsSvg) {
    std::string result = service.Generate("Hello", BarcodeType::DataMatrix, defaults);
    EXPECT_TRUE(Contains(result, "<svg"));
    EXPECT_TRUE(Contains(result, "</svg>"));
}

TEST_F(BarcodeServiceTest, ValidData_Aztec_ReturnsSvg) {
    std::string result = service.Generate("Hello", BarcodeType::Aztec, defaults);
    EXPECT_TRUE(Contains(result, "<svg"));
    EXPECT_TRUE(Contains(result, "</svg>"));
}

// 3. SVG содержит белый фон
TEST_F(BarcodeServiceTest, GeneratedSvg_ContainsWhiteBackground) {
    std::string result = service.Generate("Hello", BarcodeType::QR, defaults);
    EXPECT_TRUE(Contains(result, "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>"));
}

// 4. SVG содержит чёрные ячейки (баркод не пустой)
TEST_F(BarcodeServiceTest, GeneratedSvg_ContainsBlackCells) {
    std::string result = service.Generate("Hello", BarcodeType::QR, defaults);
    EXPECT_TRUE(Contains(result, "fill=\"black\""));
}

// 5. Разные данные → разный SVG
TEST_F(BarcodeServiceTest, DifferentData_ProduceDifferentSvg) {
    std::string result1 = service.Generate("Hello", BarcodeType::QR, defaults);
    std::string result2 = service.Generate("World", BarcodeType::QR, defaults);
    EXPECT_NE(result1, result2);
}

// 6. Одинаковые данные → одинаковый SVG (детерминированность)
TEST_F(BarcodeServiceTest, SameData_ProduceSameSvg) {
    std::string result1 = service.Generate("Hello", BarcodeType::QR, defaults);
    std::string result2 = service.Generate("Hello", BarcodeType::QR, defaults);
    EXPECT_EQ(result1, result2);
}

// 7. Размеры SVG учитывают ModuleSize и Margin
TEST_F(BarcodeServiceTest, CustomOptions_AffectSvgDimensions) {
    BarcodeOptions small;
    small.ModuleSize = 1;
    small.Margin = 0;

    BarcodeOptions large;
    large.ModuleSize = 10;
    large.Margin = 4;

    std::string smallSvg = service.Generate("Hello", BarcodeType::QR, small);
    std::string largeSvg = service.Generate("Hello", BarcodeType::QR, large);

    EXPECT_NE(smallSvg, largeSvg);
}

// 8. Разные типы баркодов → разный SVG для одних данных
TEST_F(BarcodeServiceTest, DifferentTypes_ProduceDifferentSvg) {
    std::string qr         = service.Generate("Hello", BarcodeType::QR, defaults);
    std::string dataMatrix = service.Generate("Hello", BarcodeType::DataMatrix, defaults);
    std::string aztec      = service.Generate("Hello", BarcodeType::Aztec, defaults);

    EXPECT_NE(qr, dataMatrix);
    EXPECT_NE(qr, aztec);
    EXPECT_NE(dataMatrix, aztec);
}