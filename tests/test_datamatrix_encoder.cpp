#include <iostream>
#include <fstream>
#include "datamatrix_encoder.h"
#include "svg_renderer.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    try {
        cout << "1. Генерация матрицы..." << endl;
        auto matrix = DataMatrixEncoder::encode(L"THIS_IS_KIMPINTYAO", 20, 20);

        cout << "2. Рендеринг в SVG..." << endl;
        SvgRenderer renderer;
        string svgContent = renderer.Render(matrix);

        ofstream file("barcode.svg");
        if (file.is_open()) {
            file << svgContent;
            file.close();
            cout << "Файл barcode.svg создан!" << endl;
        }
        else {
            cerr << "Не удалось создать файл!" << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}