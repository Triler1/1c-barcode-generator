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

set(SOURCES
    src/renderers/svg_renderer.cpp
    src/services/barcode_service.cpp
    src/factories/encoder_factory.cpp
    src/encoders/qr_encoder.cpp
    src/encoders/datamatrix_encoder.cpp
    src/encoders/aztec_encoder.cpp
    src/component/component_interface.cpp
)

if(WIN32)
    # Добавляем файлы линкера и ресурсов из шаблона в список сборки твоих файлов
    list(APPEND SOURCES AddInNative.rc AddInNative.def)
    
    # Добавляем обязательные для шаблона 1С макросы Windows
    add_definitions(-DUNICODE -D_UNICODE -DWIN32 -D_USRDLL -D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

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
        # Скрываем все внутренние методы, оставляя видимыми только точки входа 1С
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

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)