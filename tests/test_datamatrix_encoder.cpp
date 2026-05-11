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