#include <gtest/gtest.h>
#include "encoders/aztec_encoder.h"
#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

class AztecEncoderTest : public ::testing::Test
{
protected:
    AztecEncoder encoder;
    BarcodeOptions defaults; // ModuleSize=10, Margin=4
};

TEST_F(AztecEncoderTest, EmptyData_ThrowsInvalidArgument)
{
    EXPECT_THROW(
    {
        (void)encoder.Encode("", defaults);
    },
    std::invalid_argument
    );
}

TEST_F(AztecEncoderTest, ValidData_ReturnsNonEmptyMatrix)
{
    BarcodeMatrix result = encoder.Encode("Hello", defaults);
    EXPECT_FALSE(result.IsEmpty());
}

TEST_F(AztecEncoderTest, ValidData_MatrixIsSquare)
{
    BarcodeMatrix result = encoder.Encode("Hello", defaults);
    EXPECT_EQ(result.GetWidth(), result.GetHeight());
}

TEST_F(AztecEncoderTest, DifferentData_ProduceDifferentMatrices)
{
    BarcodeMatrix result1 = encoder.Encode("Hello", defaults);
    BarcodeMatrix result2 = encoder.Encode("World", defaults);
    bool differ = false;
    for (std::size_t y = 0; y < result1.GetHeight() && !differ; ++y)
    {
        for (std::size_t x = 0; x < result1.GetWidth() && !differ; ++x)
        {
            if (result1.Get(x, y) != result2.Get(x, y))
            {
                differ = true;
            }
        }
    }
    EXPECT_TRUE(differ);
}

TEST_F(AztecEncoderTest, SameData_ProduceSameMatrix)
{
    BarcodeMatrix result1 = encoder.Encode("Hello", defaults);
    BarcodeMatrix result2 = encoder.Encode("Hello", defaults);
    ASSERT_EQ(result1.GetWidth(), result2.GetWidth());
    ASSERT_EQ(result1.GetHeight(), result2.GetHeight());
    for (std::size_t y = 0; y < result1.GetHeight(); ++y)
    {
        for (std::size_t x = 0; x < result1.GetWidth(); ++x)
        {
            EXPECT_EQ(result1.Get(x, y), result2.Get(x, y));
        }
    }
}

TEST_F(AztecEncoderTest, LongerData_ProducesLargerOrEqualMatrix)
{
    BarcodeMatrix shortResult = encoder.Encode("Hi", defaults);
    BarcodeMatrix longResult = encoder.Encode(std::string(200, 'A'), defaults);
    EXPECT_GE(longResult.GetWidth(), shortResult.GetWidth());
}