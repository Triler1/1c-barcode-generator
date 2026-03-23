#include "SvgRenderer.h"
#include <BitMatrixIO.h>

std::string SvgRenderer::Render(const ZXing::BitMatrix& matrix) {
    return ZXing::ToSVG(matrix);
}