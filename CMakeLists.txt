cmake_minimum_required(VERSION 3.16)
project(barcode_generator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZXing REQUIRED)

add_library(barcode_generator
    src/renderers/svg_renderer.cpp
    src/services/barcode_service.cpp
    src/factories/encoder_factory.cpp
    src/encoders/qr_encoder.cpp
    src/encoders/datamatrix_encoder.cpp
    src/encoders/aztec_encoder.cpp
)

target_include_directories(barcode_generator PUBLIC include)
target_link_libraries(barcode_generator PRIVATE ZXing::ZXing)