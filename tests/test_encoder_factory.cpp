#include <gtest/gtest.h>
#include "factories/encoder_factory.h"
#include "common/barcode_types.h"
#include "common/barcode_matrix.h"
#include "common/barcode_options.h"

class EncoderFactoryTest : public ::testing::Test {
protected:
    BarcodeOptions defaults;
};

TEST_F(EncoderFactoryTest, CreateAztec_ReturnsValidEncoder) {
    auto encoder = EncoderFactory::Create(BarcodeType::Aztec);
    EXPECT_NE(encoder, nullptr);
}

TEST_F(EncoderFactoryTest, CreateQR_ReturnsValidEncoder) {
    auto encoder = EncoderFactory::Create(BarcodeType::QR);
    EXPECT_NE(encoder, nullptr);
}

