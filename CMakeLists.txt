cmake_minimum_required(VERSION 3.20)
project(1c-barcode-generator LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(external/zxing-cpp EXCLUDE_FROM_ALL)

set(ZXING_USE_BUNDLED_ZINT OFF CACHE BOOL "" FORCE)

set(BUILD_WRITERS ON  CACHE BOOL "" FORCE)
set(BUILD_READERS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS    OFF CACHE BOOL "" FORCE)

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

# Надёжное подключение заголовков (работает на всех ОС)
target_include_directories(${PROJECT_NAME} PRIVATE
    $<TARGET_PROPERTY:ZXing::ZXing,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(${PROJECT_NAME} PRIVATE ZXing::ZXing)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "BarcodeGenerator"
        PREFIX ""
        SUFFIX ".dll"
    )
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "BarcodeGenerator"
        PREFIX "lib"
        SUFFIX ".dylib"
    )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "BarcodeGenerator"
        PREFIX "lib"
        SUFFIX ".so"
    )
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)