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
TEST_F(DataMatrixEncoderTest, SameData_ProduceSameMatrix) {
    BarcodeMatrix result1 = encoder.Encode("Test", defaults);
    BarcodeMatrix result2 = encoder.Encode("Test", defaults);
    ASSERT_EQ(result1.GetWidth(), result2.GetWidth());
    ASSERT_EQ(result1.GetHeight(), result2.GetHeight());
    for (std::size_t y = 0; y < result1.GetHeight(); ++y)
        for (std::size_t x = 0; x < result1.GetWidth(); ++x)
            EXPECT_EQ(result1.Get(x, y), result2.Get(x, y));
}
TEST_F(DataMatrixEncoderTest, LongerData_ProducesLargerOrEqualMatrix) {
    BarcodeMatrix shortResult = encoder.Encode("Hi", defaults);
    BarcodeMatrix longResult  = encoder.Encode(std::string(200, 'A'), defaults);
    EXPECT_GE(longResult.GetWidth(), shortResult.GetWidth());
}
TEST_F(DataMatrixEncoderTest, SpecialCharacters_DoNotCrash) {
    EXPECT_NO_THROW(encoder.Encode("Привет\nWorld\t!", defaults));
    BarcodeMatrix result = encoder.Encode("Привет\nWorld\t!", defaults);
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_GT(result.GetWidth(), 0u);
}
TEST_F(DataMatrixEncoderTest, VeryLongInput_DoesNotCrash) {
    std::string longData(5000, 'A');
    BarcodeMatrix result;
    EXPECT_NO_THROW(result = encoder.Encode(longData, defaults));
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_GT(result.GetWidth(), 0u);
}
TEST_F(DataMatrixEncoderTest, Matrix_HasAtLeastOneBlackCell) {
    BarcodeMatrix result = encoder.Encode("Some data", defaults);
    bool hasBlack = false;
    for (std::size_t y = 0; y < result.GetHeight(); ++y)
        for (std::size_t x = 0; x < result.GetWidth(); ++x)
            if (result.Get(x, y))
                hasBlack = true;
    EXPECT_TRUE(hasBlack);
}
TEST_F(DataMatrixEncoderTest, Matrix_IsNotCompletelyBlack) {
    BarcodeMatrix result = encoder.Encode("Some data", defaults);
    bool hasWhite = false;
    for (std::size_t y = 0; y < result.GetHeight(); ++y)
        for (std::size_t x = 0; x < result.GetWidth(); ++x)
            if (!result.Get(x, y))
                hasWhite = true;
    EXPECT_TRUE(hasWhite);