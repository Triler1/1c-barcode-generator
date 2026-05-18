#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "renderers/svg_renderer.h"
#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

static bool Contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

static std::size_t CountOccurrences(const std::string& haystack, const std::string& needle) {
    std::size_t count = 0;
    std::size_t pos = 0;

    while ((pos = haystack.find(needle, pos)) != std::string::npos) {
        ++count;
        ++pos;
    }

    return count;
}

class SvgRendererTest : public ::testing::Test {
protected:
    SvgRenderer renderer;
    BarcodeOptions defaults; // ModuleSize=10, Margin=4
};

// 1. Пустая матрица → фиксированная строка
TEST_F(SvgRendererTest, EmptyMatrix_ReturnsEmptySvg) {
    BarcodeMatrix empty;

    EXPECT_EQ(
        renderer.Render(empty, defaults),
        "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        "width=\"0\" height=\"0\" viewBox=\"0 0 0 0\"></svg>"
    );
}

// 2. Размеры SVG считаются правильно: (width + 2*margin) * moduleSize
// (3 + 2*2) * 5 = 35, (2 + 2*2) * 5 = 30
TEST_F(SvgRendererTest, Dimensions_AreCalculatedCorrectly) {
    BarcodeMatrix m(3, 2);

    BarcodeOptions o;
    o.ModuleSize = 5;
    o.Margin = 2;

    std::string result = renderer.Render(m, o);

    EXPECT_TRUE(Contains(result, "width=\"35\""));
    EXPECT_TRUE(Contains(result, "height=\"30\""));
    EXPECT_TRUE(Contains(result, "viewBox=\"0 0 35 30\""));
}

// 3. Белый фон всегда присутствует
TEST_F(SvgRendererTest, WhiteBackground_AlwaysPresent) {
    BarcodeMatrix m(2, 2);

    std::string result = renderer.Render(m, defaults);

    EXPECT_TRUE(Contains(
        result,
        "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>"
    ));
}

// 4. Матрица без чёрных ячеек — нет ни одного fill="black"
TEST_F(SvgRendererTest, AllWhiteCells_NoBlackRects) {
    BarcodeMatrix m(3, 3);

    std::string result = renderer.Render(m, defaults);

    EXPECT_FALSE(Contains(result, "fill=\"black\""));
}

// 5. Координаты чёрной ячейки считаются правильно
// x=1, y=2, margin=1, moduleSize=3 → rectX=(1+1)*3=6, rectY=(2+1)*3=9
TEST_F(SvgRendererTest, BlackCell_CorrectCoordinates) {
    BarcodeMatrix m(5, 5);
    m.Set(1, 2, true);

    BarcodeOptions o;
    o.ModuleSize = 3;
    o.Margin = 1;

    std::string result = renderer.Render(m, o);

    EXPECT_TRUE(Contains(
        result,
        "<rect x=\"6\" y=\"9\" width=\"3\" height=\"3\" fill=\"black\"/>"
    ));
}

// 6. Одна чёрная ячейка — ровно один fill="black"
TEST_F(SvgRendererTest, OneBlackCell_ExactlyOneBlackRect) {
    BarcodeMatrix m(3, 3);
    m.Set(0, 0, true);

    std::string result = renderer.Render(m, defaults);

    EXPECT_EQ(CountOccurrences(result, "fill=\"black\""), 1u);
}

// 7. Несколько чёрных ячеек — количество rect совпадает
TEST_F(SvgRendererTest, MultipleBlackCells_CorrectCount) {
    BarcodeMatrix m(3, 3);
    m.Set(0, 0, true);
    m.Set(1, 1, true);
    m.Set(2, 2, true);

    std::string result = renderer.Render(m, defaults);

    EXPECT_EQ(CountOccurrences(result, "fill=\"black\""), 3u);
}

// 8. Выход за границы матрицы бросает исключение
TEST_F(SvgRendererTest, OutOfBounds_ThrowsException) {
    BarcodeMatrix m(2, 2);

    EXPECT_THROW(
        {
            (void)m.Get(5, 5);
        },
        std::out_of_range
    );

    EXPECT_THROW(
        {
            m.Set(5, 5, true);
        },
        std::out_of_range
    );
}