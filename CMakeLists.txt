cmake_minimum_required(VERSION 3.20)
project(1c-barcode-generator LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
    zxing
    GIT_REPOSITORY https://github.com/zxing-cpp/zxing-cpp.git
    GIT_TAG        v3.0.2
    GIT_SHALLOW    TRUE
)

set(BUILD_WRITERS ON  CACHE BOOL "Build barcode writers" FORCE)
set(BUILD_READERS ON  CACHE BOOL "Build barcode readers" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(BUILD_TESTS    OFF CACHE BOOL "Build tests" FORCE)

FetchContent_MakeAvailable(zxing)

set(SOURCES
    src/renderers/svg_renderer.cpp
    src/services/barcode_service.cpp
    src/factories/encoder_factory.cpp
    src/encoders/qr_encoder.cpp
    src/encoders/datamatrix_encoder.cpp
    src/encoders/aztec_encoder.cpp
    src/component/component_interface.cpp
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