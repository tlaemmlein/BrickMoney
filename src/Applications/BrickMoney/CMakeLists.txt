cmake_minimum_required(VERSION 3.1)

project(BrickMoney LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick Widgets REQUIRED)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

add_executable(${PROJECT_NAME} 
images/images.qrc
main.cpp 
qml.qrc
TableModel.cpp
TableModel.h
LegoSetIOManager.cpp
LegoSetIOManager.h)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::Widgets)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/../../Thirdparty/OpenSSL/1.1.1d/vs2017-x64/bin/libcrypto-1_1-x64.dll
                ${CMAKE_CURRENT_BINARY_DIR}/libcrypto-1_1-x64.dll)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/../../Thirdparty/OpenSSL/1.1.1d/vs2017-x64/bin/libssl-1_1-x64.dll
                ${CMAKE_CURRENT_BINARY_DIR}/libssl-1_1-x64.dll)

