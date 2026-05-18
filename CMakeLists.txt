cmake_minimum_required(VERSION 3.20)
project(1c-barcode-generator LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(WIN32)
    enable_language(RC)
endif()

set(ZXING_USE_BUNDLED_ZINT ON CACHE BOOL "" FORCE)
set(BUILD_WRITERS ON  CACHE BOOL "" FORCE)
set(BUILD_READERS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS    OFF CACHE BOOL "" FORCE)

add_subdirectory(external/zxing-cpp EXCLUDE_FROM_ALL)

set(BARCODE_CORE_SOURCES
    src/renderers/svg_renderer.cpp
    src/services/barcode_service.cpp
    src/factories/encoder_factory.cpp
    src/encoders/qr_encoder.cpp
    src/encoders/datamatrix_encoder.cpp
    src/encoders/aztec_encoder.cpp
)

set(COMPONENT_SOURCES
    ${BARCODE_CORE_SOURCES}
    src/component/component_interface.cpp
    src/dllmain.cpp
)

if(WIN32)
    list(APPEND COMPONENT_SOURCES
        AddInNative.rc
        AddInNative.def
    )
endif()

if (UNIX)
    if (APPLE)
        ADD_DEFINITIONS(-D__APPLE__ )
    endif()
else()
    add_definitions(-DUNICODE -D_UNICODE -DWIN32 -D_USRDLL -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -DADDINCPP_EXPORTS)
endif()

add_library(${PROJECT_NAME} SHARED ${COMPONENT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME} PRIVATE
    $<TARGET_PROPERTY:ZXing::ZXing,INTERFACE_INCLUDE_DIRECTORIES>
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/1c-sdk/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ZXing::ZXing)

if(UNIX)
    if(APPLE)
        target_link_libraries(${PROJECT_NAME} PRIVATE iconv)
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
    endif()
endif()

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

IF (UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,--no-undefined -Xlinker --version-script -Xlinker ${CMAKE_CURRENT_SOURCE_DIR}/version.script" )
ENDIF()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

option(BARCODE_GENERATOR_BUILD_TESTS "Build unit tests" ON)

if(BARCODE_GENERATOR_BUILD_TESTS)
    include(FetchContent)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(${PROJECT_NAME}-tests
        tests/test_qr_encoder.cpp
        tests/test_datamatrix_encoder.cpp
        tests/test_aztec_encoder.cpp
        tests/test_encoder_factory.cpp
        tests/test_barcode_service.cpp
        tests/test_svg_renderer.cpp

        ${BARCODE_CORE_SOURCES}
    )

    target_include_directories(${PROJECT_NAME}-tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(${PROJECT_NAME}-tests
        PRIVATE
            ZXing::ZXing
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}-tests)
endif()