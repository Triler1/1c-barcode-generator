#include <gtest/gtest.h>
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
TEST_F(DataMatrixEncoderTest, EmptyData_ThrowsInvalidArgument) {
    EXPECT_THROW(encoder.Encode("", defaults), std::invalid_argument);
}
TEST_F(DataMatrixEncoderTest, ValidData_ReturnsNonEmptyMatrix) {
    BarcodeMatrix result = encoder.Encode("Hello", defaults);
    EXPECT_FALSE(result.IsEmpty());
}
TEST_F(DataMatrixEncoderTest, ValidData_MatrixHasPositiveDimensions) {
    BarcodeMatrix result = encoder.Encode("Test", defaults);
    EXPECT_GT(result.GetWidth(), 0u);
    EXPECT_GT(result.GetHeight(), 0u);
}
TEST_F(DataMatrixEncoderTest, DifferentData_ProduceDifferentMatrices) {
    BarcodeMatrix result1 = encoder.Encode("Hello", defaults);
    BarcodeMatrix result2 = encoder.Encode("World", defaults);
    bool differ = false;
    for (std::size_t y = 0; y < result1.GetHeight() && !differ; ++y)
        for (std::size_t x = 0; x < result1.GetWidth() && !differ; ++x)
            if (result1.Get(x, y) != result2.Get(x, y))
                differ = true;
    EXPECT_TRUE(differ);
}