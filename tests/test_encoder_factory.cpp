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

TEST_F(EncoderFactoryTest, CreateDataMatrix_ReturnsValidEncoder) {
    auto encoder = EncoderFactory::Create(BarcodeType::DataMatrix);
    EXPECT_NE(encoder, nullptr);
}

TEST_F(EncoderFactoryTest, AztecEncoder_ProducesNonEmptyMatrix) {
    auto encoder = EncoderFactory::Create(BarcodeType::Aztec);
    BarcodeMatrix result = encoder->Encode("HELLO_FACTORY", defaults);
    EXPECT_FALSE(result.IsEmpty());
}

TEST_F(EncoderFactoryTest, QREncoder_ProducesNonEmptyMatrix) {
    auto encoder = EncoderFactory::Create(BarcodeType::QR);
    BarcodeMatrix result = encoder->Encode("HELLO_FACTORY", defaults);
    EXPECT_FALSE(result.IsEmpty());
}

TEST_F(EncoderFactoryTest, DataMatrixEncoder_ProducesNonEmptyMatrix) {
    auto encoder = EncoderFactory::Create(BarcodeType::DataMatrix);
    BarcodeMatrix result = encoder->Encode("HELLO_FACTORY", defaults);
    EXPECT_FALSE(result.IsEmpty());
}