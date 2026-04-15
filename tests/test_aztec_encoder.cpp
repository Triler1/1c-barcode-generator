#include <iostream>
#include "aztec_encoder.h"
#include "svg_renderer.h"


int main() {
    std::cout << "Test started" << std::endl;
    AztecEncoder encoder;
    auto matrix = encoder.Encode("AZTEC");

    return 0;
}