#include <gtest/gtest.h>
#include "encoders/aztec_encoder.h"
#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

class AztecEncoderTest : public ::testing::Test {
protected:
    AztecEncoder encoder;
    BarcodeOptions defaults; // ModuleSize=10, Margin=4
};

TEST_F(AztecEncoderTest, EmptyData_ThrowsInvalidArgument) {
    EXPECT_THROW(encoder.Encode("", defaults), std::invalid_argument);
}

TEST_F(AztecEncoderTest, ValidData_ReturnsNonEmptyMatrix) {
    BarcodeMatrix result = encoder.Encode("Hello", defaults);
    EXPECT_FALSE(result.IsEmpty());
}

TEST_F(AztecEncoderTest, ValidData_MatrixIsSquare) {
    BarcodeMatrix result = encoder.Encode("Hello", defaults);
    EXPECT_EQ(result.GetWidth(), result.GetHeight());
}