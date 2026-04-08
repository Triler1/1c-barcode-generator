#include "renderers/svg_renderer.h"

#include <sstream>

std::string SvgRenderer::Render(const BarcodeMatrix& matrix, const BarcodeOptions& options) const {
    if (matrix.IsEmpty()) {
        return "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"0\" height=\"0\" viewBox=\"0 0 0 0\"></svg>";
    }
    const std::size_t moduleSize = options.ModuleSize;
    const std::size_t margin = options.Margin;

    const std::size_t svgWidth = (matrix.GetWidth() + 2 * margin) * moduleSize;
    const std::size_t svgHeight = (matrix.GetHeight() + 2 * margin) * moduleSize;

    std::ostringstream svg;
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" " << "width=\"" << svgWidth << "\" " << "height=\"" << svgHeight << "\" " << "viewBox=\"0 0 " << svgWidth << " " << svgHeight << "\">";
    svg << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>";
    for (std::size_t y = 0; y < matrix.GetHeight(); ++y) {
        for (std::size_t x = 0; x < matrix.GetWidth(); ++x) {
            if (matrix.Get(x, y)) {
                const std::size_t rectX = (x + margin) * moduleSize;
                const std::size_t rectY = (y + margin) * moduleSize;
                svg << "<rect x=\"" << rectX << "\" y=\"" << rectY << "\" width=\"" << moduleSize << "\" height=\"" << moduleSize << "\" fill=\"black\"/>";
            }
        }
    }
    svg << "</svg>";
    return svg.str();
}