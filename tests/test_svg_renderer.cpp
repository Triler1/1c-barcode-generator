#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "renderers/svg_renderer.h"

// Minimal stubs — удали если в проекте уже есть реальные заголовки

struct BarcodeOptions {
    std::size_t ModuleSize = 1;
    std::size_t Margin     = 0;
};

class BarcodeMatrix {
public:
    BarcodeMatrix() = default;

    BarcodeMatrix(std::size_t width, std::size_t height)
        : width_(width), height_(height), data_(width * height, false) {}

    void Set(std::size_t x, std::size_t y, bool value) {
        data_[y * width_ + x] = value;
    }

    bool        Get(std::size_t x, std::size_t y) const { return data_[y * width_ + x]; }
    std::size_t GetWidth()  const { return width_;  }
    std::size_t GetHeight() const { return height_; }
    bool        IsEmpty()   const { return width_ == 0 || height_ == 0; }

private:
    std::size_t width_  = 0;
    std::size_t height_ = 0;
    std::vector<bool> data_;
};


static bool Contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

class SvgRendererTest : public ::testing::Test {
protected:
    SvgRenderer    renderer;
    BarcodeOptions defaults;
};

// 1. Пустая матрица → фиксированная строка

TEST_F(SvgRendererTest, EmptyMatrix_ReturnsEmptySvg) {
    BarcodeMatrix empty;
    EXPECT_EQ(renderer.Render(empty, defaults),
        "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        "width=\"0\" height=\"0\" viewBox=\"0 0 0 0\"></svg>");
}

// 2. Корректные размеры с учётом margin и moduleSize

TEST_F(SvgRendererTest, Dimensions_MarginAndModuleSize) {
    // (3 + 2*2) * 5 = 35  по ширине,  (2 + 2*2) * 5 = 30 по высоте
    BarcodeMatrix m(3, 2);
    BarcodeOptions o; o.ModuleSize = 5; o.Margin = 2;
    std::string result = renderer.Render(m, o);

    EXPECT_TRUE(Contains(result, "width=\"35\""));
    EXPECT_TRUE(Contains(result, "height=\"30\""));
    EXPECT_TRUE(Contains(result, "viewBox=\"0 0 35 30\""));
}

// 3. Белый фон всегда присутствует

TEST_F(SvgRendererTest, AlwaysHasWhiteBackground) {
    BarcodeMatrix m(2, 2);
    EXPECT_TRUE(Contains(renderer.Render(m, defaults),
        "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>"));
}

// 4. Белые ячейки не генерируют чёрных прямоугольников

TEST_F(SvgRendererTest, AllWhiteCells_NoBlackRect) {
    BarcodeMatrix m(3, 3); // все false
    EXPECT_FALSE(Contains(renderer.Render(m, defaults), "fill=\"black\""));
}

// 5. Координаты чёрной ячейки с учётом margin и moduleSize

TEST_F(SvgRendererTest, BlackCell_CorrectPosition) {
    BarcodeMatrix m(5, 5);
    m.Set(1, 2, true);
    BarcodeOptions o; o.ModuleSize = 3; o.Margin = 1;

    EXPECT_TRUE(Contains(renderer.Render(m, o),
        "<rect x=\"6\" y=\"9\" width=\"3\" height=\"3\" fill=\"black\"/>"));
}