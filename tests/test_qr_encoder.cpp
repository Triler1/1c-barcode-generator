#include "encoders/qr_encoder.h"

#include <ZXing/QRCode/QRErrorCorrectionLevel.h>

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

using ECL = ZXing::QRCode::ErrorCorrectionLevel;

class QREncoderTest : public ::testing::Test {
protected:
    QREncoder encoder;
};

TEST(QREncoderConstructionTest, DefaultConstructorSucceeds)
{
    EXPECT_NO_THROW(QREncoder());
}

TEST(QREncoderConstructionTest, ExplicitParametersSucceed)
{
    EXPECT_NO_THROW(QREncoder(ECL::High, 5));
}

TEST(QREncoderConstructionTest, AutoVersionZeroIsAllowed)
{
    EXPECT_NO_THROW(QREncoder(ECL::Medium, 0));
}

TEST(QREncoderConstructionTest, VersionBoundariesAreAccepted)
{
    EXPECT_NO_THROW(QREncoder(ECL::Low, 1));
    EXPECT_NO_THROW(QREncoder(ECL::Low, 40));
}

TEST(QREncoderConstructionTest, InvalidVersionThrowsInvalidArgument)
{
    EXPECT_THROW(QREncoder(ECL::Medium, -1),  std::invalid_argument);
    EXPECT_THROW(QREncoder(ECL::Medium, 41),  std::invalid_argument);
    EXPECT_THROW(QREncoder(ECL::Medium, 100), std::invalid_argument);
}

TEST_F(QREncoderTest, EncodeReturnsNonEmptyBitMatrix)
{
    ZXing::BitMatrix matrix = encoder.Encode("Hello, World!");
    EXPECT_FALSE(matrix.empty());
    EXPECT_GT(matrix.width(),  0);
    EXPECT_GT(matrix.height(), 0);
}

TEST_F(QREncoderTest, QRCodeSymbolIsSquare)
{
    auto matrix = encoder.Encode("test");
    EXPECT_EQ(matrix.width(), matrix.height());
}

TEST_F(QREncoderTest, ReturnTypeIsZXingBitMatrix)
{
    static_assert(
        std::is_same_v<decltype(encoder.Encode("")), ZXing::BitMatrix>,
        "Encode must return ZXing::BitMatrix to satisfy IEncoder contract");
}

TEST(QREncoderInterfaceTest, IsAssignableToIEncoderPointer)
{
    IEncoder* enc = new QREncoder(ECL::Medium, 0);
    ZXing::BitMatrix matrix = enc->Encode("interface-check");
    EXPECT_FALSE(matrix.empty());
    delete enc;
}

TEST_F(QREncoderTest, EmptyDataThrowsInvalidArgument)
{
    EXPECT_THROW(encoder.Encode(""), std::invalid_argument);
}

class QREncoderEcLevelTest : public ::testing::TestWithParam<ECL> {};

INSTANTIATE_TEST_SUITE_P(AllLevels, QREncoderEcLevelTest,
    ::testing::Values(ECL::Low, ECL::Medium, ECL::Quartile, ECL::High));

TEST_P(QREncoderEcLevelTest, EncodeSucceedsForAllEcLevels)
{
    QREncoder enc(GetParam(), 0);
    auto matrix = enc.Encode("test");
    EXPECT_FALSE(matrix.empty());
}

TEST(QREncoderEcTest, HigherEcLevelProducesLargerOrEqualSymbol)
{
    const std::string data = "https://example.com";
    QREncoder encL(ECL::Low,  0);
    QREncoder encH(ECL::High, 0);
    EXPECT_GE(encH.Encode(data).width(), encL.Encode(data).width());
}

TEST(QREncoderVersionTest, ExplicitVersionProducesCorrectSymbolSize)
{
    const int version  = 3;
    const int expected = 17 + 4 * version;  

    QREncoder enc(ECL::Medium, version);
    auto matrix = enc.Encode("short");
    EXPECT_EQ(matrix.width(),  expected);
    EXPECT_EQ(matrix.height(), expected);
}

TEST(QREncoderVersionTest, AutoVersionSelectsSmallestFittingSymbol)
{
    QREncoder encAuto(ECL::Medium, 0);
    QREncoder encV10 (ECL::Medium, 10);

    auto matAuto = encAuto.Encode("A");
    auto matV10  = encV10 .Encode("A");

    EXPECT_LE(matAuto.width(), matV10.width());
}

TEST_F(QREncoderTest, EncodeURL)
{
    EXPECT_NO_THROW(encoder.Encode("https://example.com/path?q=1&r=2"));
}

TEST_F(QREncoderTest, EncodeNumericPayload)
{
    EXPECT_NO_THROW(encoder.Encode("0123456789"));
}

TEST_F(QREncoderTest, EncodeLongPayload)
{
    QREncoder enc(ECL::Low, 0);
    EXPECT_NO_THROW(enc.Encode(std::string(1000, 'A')));
}

TEST_F(QREncoderTest, EncodeUnicodePayload)
{
    EXPECT_NO_THROW(encoder.Encode("こんにちは世界"));
}

TEST_F(QREncoderTest, SameInputProducesSameBitMatrix)
{
    const std::string data = "determinism-test";
    auto m1 = encoder.Encode(data);
    auto m2 = encoder.Encode(data);

    ASSERT_EQ(m1.width(),  m2.width());
    ASSERT_EQ(m1.height(), m2.height());

    for (int y = 0; y < m1.height(); ++y)
        for (int x = 0; x < m1.width(); ++x)
            EXPECT_EQ(m1.get(x, y), m2.get(x, y))
                << "Mismatch at (" << x << ", " << y << ")";
}

TEST_F(QREncoderTest, FinderPatternTopLeftCornerIsDark)
{
    auto matrix = encoder.Encode("finder-pattern");
    EXPECT_TRUE(matrix.get(0, 0))
        << "Expected dark module at top-left finder pattern origin";
}
