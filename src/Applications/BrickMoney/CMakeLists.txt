cmake_minimum_required(VERSION 3.1)

project(BrickMoney LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 Widgets REQUIRED)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/../../Thirdparty)

find_package(log4cplus HINTS ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/)

set(QML_FILES
    qml/LegoList.qml
    qml/LegoTable.qml
    qml/main.qml
    qml/ForSale.qml
    qml/InStock.qml
    qml/Overview.qml
    qml/Sold.qml
)

set(QML_HELPER_FILES
    qml/helper/DatePicker.qml
    qml/helper/DoubleSpinBox.qml
    qml/helper/NewLegoSetDialog.qml
    qml/helper/SortableColumnHeading.qml
)

set(QML_SANDBOX_FILES
    qml/sandbox/DatePickerTest.qml
    qml/sandbox/DialogButtonBoxTest.qml
    qml/sandbox/ListViewScrollbars.qml
)

set(SRC_FILES
    src/BrickMoneySettings.cpp
    src/BrickMoneySettings.h
    src/BrickMoneyProject.cpp
    src/BrickMoneyProject.h
    src/LegoSetDataSource.cpp
    src/LegoSetDataSource.h
    src/LegoSet.cpp
    src/LegoSet.h
    src/LegoSetInfoGenerator.cpp
    src/LegoSetInfoGenerator.h
    src/LegoSetTableModel.cpp
    src/LegoSetTableModel.h
    src/LegoSetSortFilterTableModel.cpp
    src/LegoSetSortFilterTableModel.h
    src/Logging.h
    src/magic_enum.h
    src/main.cpp
    src/QmlValuePreview.h
)

add_executable(${PROJECT_NAME}
    images/images.qrc
    qml/qml.qrc
    ${SRC_FILES}
    ${QML_FILES}
    ${QML_HELPER_FILES}
    ${QML_SANDBOX_FILES}
    images/BrickMoney.rc
)

source_group("Qml Files" FILES ${QML_FILES})
source_group("Qml Files" FILES ${QML_FILES})
source_group("Qml/helper Files" FILES ${QML_HELPER_FILES})
source_group("Qml/sandbox Files" FILES ${QML_SANDBOX_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::QuickControls2 Qt5::Widgets)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${_qt5Core_install_prefix}/bin/windeployqt.exe
  --verbose 0
  --force
  --no-compiler-runtime
  --qmldir ${_qt5Core_install_prefix}/qml
   $<TARGET_FILE:BrickMoney>
   # windeployqt is not able to deploy Qt/labs/platform! Hence we do it by own.
COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${_qt5Core_install_prefix}/qml/Qt/labs/platform
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt/labs/platform
  # windeployqt is not able to deploy Qt/labs/qmlmodels! Hence we do it by own.
COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${_qt5Core_install_prefix}/qml/Qt/labs/qmlmodels
       $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt/labs/qmlmodels
         )

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${THIRDPARTY_DIR}/OpenSSL/1.1.1d/vs2017-x64/bin/libcrypto-1_1-x64.dll
                $<TARGET_FILE_DIR:BrickMoney>/libcrypto-1_1-x64.dll)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${THIRDPARTY_DIR}/OpenSSL/1.1.1d/vs2017-x64/bin/libssl-1_1-x64.dll
                $<TARGET_FILE_DIR:BrickMoney>/libssl-1_1-x64.dll)

target_link_libraries(${PROJECT_NAME} PUBLIC log4cplus::log4cplusU)
target_include_directories(${PROJECT_NAME} PUBLIC log4cplus::log4cplusU)

set( LOG4CPLUS_DLLS
   ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/bin/log4cplusU.dll
   ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/bin/log4cplusUD.dll
)

foreach( file_i ${LOG4CPLUS_DLLS})
    add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${file_i} $<TARGET_FILE_DIR:BrickMoney>
)
endforeach( file_i )

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "DEBUG_SUFFIX: ${DEBUG_SUFFIX}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "_qt5Core_install_prefix: ${_qt5Core_install_prefix}")
message(STATUS "Qt5_DIR: ${Qt5_DIR}")
message(STATUS "THIRDPARTY_DIR: ${THIRDPARTY_DIR}")
message(STATUS "log4cplus_DIR: ${log4cplus_DIR}")

