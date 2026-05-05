cmake_minimum_required(VERSION 3.20)
project(1c-barcode-generator LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

find_package(ZXing CONFIG REQUIRED)

set(SOURCES
    src/renderers/svg_renderer.cpp
    src/services/barcode_service.cpp
    src/factories/encoder_factory.cpp
    src/encoders/qr_encoder.cpp
    src/encoders/datamatrix_encoder.cpp
    src/encoders/aztec_encoder.cpp
    src/component/component_interface.cpp          # ← добавьте, если файла ещё нет
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ZXing::ZXing)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "BarcodeGenerator"      # ← имя DLL без .dll
        PREFIX ""
        SUFFIX ".dll"
    )
elseif(UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "BarcodeGenerator"
        PREFIX "lib"
        SUFFIX ".so"
    )
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "BarcodeGenerator"
        PREFIX "lib"
        SUFFIX ".dylib"
    )
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)