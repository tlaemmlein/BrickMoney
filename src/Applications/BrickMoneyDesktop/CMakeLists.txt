cmake_minimum_required(VERSION 3.1)

project(BrickMoneyDesktop LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty)

set(SRC_FILES
    src/main.cpp
    src/InStock.h
    src/InStock.cpp
    src/InStock.ui
    src/ForSale.h
    src/ForSale.cpp
    src/ForSale.ui
    src/Sold.h
    src/Sold.cpp
    src/Sold.ui
    src/MainWindow.h
    src/MainWindow.cpp
    src/MainWindow.ui
    src/ImageDelegate.cpp
    src/ImageDelegate.h
    src/CheckBoxDelegate.cpp
    src/CheckBoxDelegate.h
    )

add_executable(${PROJECT_NAME}
    images/BrickMoney.rc
    images/images.qrc
    ${SRC_FILES}
)


target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets
    BrickMoney::Logging BrickMoney::BrickMoneyData BrickMoney::LegoSetModels BrickMoney::LegoSetInfoGenerator
    BrickMoney::BrickMoneyProject)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_SOURCE_DIR}/../)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../Packages/Logging/Logging.cmake)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${_qt5Core_install_prefix}/bin/windeployqt.exe
  --verbose 0
  --force
  --no-compiler-runtime
  --qmldir ${_qt5Core_install_prefix}/qml
   $<TARGET_FILE:${PROJECT_NAME}>
   # windeployqt is not able to deploy Qt/labs/platform! Hence we do it by own.
COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${_qt5Core_install_prefix}/qml/Qt/labs/platform
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt/labs/platform
         )

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${THIRDPARTY_DIR}/OpenSSL/1.1.1d/vs2017-x64/bin/libcrypto-1_1-x64.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/libcrypto-1_1-x64.dll)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${THIRDPARTY_DIR}/OpenSSL/1.1.1d/vs2017-x64/bin/libssl-1_1-x64.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/libssl-1_1-x64.dll)

set( LOG4CPLUS_DLLS
   ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/bin/log4cplusU.dll
   ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/bin/log4cplusUD.dll
)

foreach( file_i ${LOG4CPLUS_DLLS})
    add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${file_i} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
endforeach( file_i )


message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "_qt5Core_install_prefix: ${_qt5Core_install_prefix}")
message(STATUS "Qt5_DIR: ${Qt5_DIR}")
message(STATUS "THIRDPARTY_DIR: ${THIRDPARTY_DIR}")
message(STATUS "TARGET_FILE_DIR: $<TARGET_FILE_DIR:${PROJECT_NAME}>")


