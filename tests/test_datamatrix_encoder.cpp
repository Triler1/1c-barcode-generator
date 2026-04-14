#include <iostream>
#include <fstream>
#include "datamatrix_encoder.h"
#include "svg_renderer.h"

using namespace std;

int main()
{
    try {
        cout << "[INFO] Generating matrix (std::string input)..." << endl;

        auto matrix = DataMatrixEncoder::encode("THIS_IS_KIMPINTYAO");

        cout << "[INFO] Rendering SVG..." << endl;

        SvgRenderer renderer;
        string svgContent = renderer.Render(matrix);

        ofstream file("barcode.svg");
        if (!file) {
            throw runtime_error("Cannot open output file");
        }

        file << svgContent;

        cout << "[SUCCESS] barcode.svg created!" << endl;
    }
    catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        return 1;
    }

    return 0;
}