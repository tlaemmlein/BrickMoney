cmake_minimum_required(VERSION 3.1)

project(BrickMoneyDesktop LANGUAGES CXX)

# Managing translations require LinguistTools module
# The cmake function is defined in the Qt installation tree:
# i.e. Qt5.9.1/5.9.1/gcc_64/lib/cmake/Qt5LinguistTools/Qt5LinguistToolsMacros.cmake
# Reference: https://doc.qt.io/qt-5/cmake-manual.html#qt5linguisttools-macros
find_package(Qt5 COMPONENTS Core Gui Widgets LinguistTools Sql REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty)

set(SRC_FILES
    src/AboutDialog.cpp
    src/AboutDialog.h
    src/AboutDialog.ui
	src/BrickMoneyTranslator.cpp
	src/BrickMoneyTranslator.h
    src/main.cpp
    src/InStock.h
    src/InStock.cpp
    src/ForSale.h
    src/ForSale.cpp
    src/Sold.h
    src/Sold.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/MainWindow.ui
    src/CalendarDelegate.h
    src/CalendarDelegate.cpp
    src/ImageDelegate.cpp
    src/ImageDelegate.h
    src/LineEditDelegate.cpp
    src/LineEditDelegate.h
    src/CheckBoxDelegate.cpp
    src/CheckBoxDelegate.h
    src/DoubleSpinBoxDelegate.cpp
    src/DoubleSpinBoxDelegate.h
    src/LegoSetSpinBoxDelegate.cpp
    src/LegoSetSpinBoxDelegate.h
    src/LegSetTableView.cpp
    src/LegSetTableView.h
    src/LegSetTableView.ui
    src/LegoSetImportDialog.cpp
    src/LegoSetImportDialog.h
    src/LegoSetImportDialog.ui
    )

set(TS_FILES
    translations/bm_de.ts
    )

# https://stackoverflow.com/questions/44782914/cmake-qt5-add-translation-how-to-specify-the-output-path
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/translations)    

# Call lupdate to generate (or update already existing) ts files from source
# code (.cpp and .ui) code AND generate qm files. We can call it with a source
# directory OR individual files to search for strings to translate. The ts files
# are generated in the source directory and the qm files are created in the
# build directory, as it should, because we don't want to pollute our source
# directory with generated binary files.
#
# Recall that the ts files are edited by translators, while qm files are just
# binary representation of the ts file for perfomance and size optimization.
#
# Using a source file list:
# qt5_create_translation(QM_FILES
#     translatemainwindow.cpp translatemainwindow.ui
#     english.ts french.ts
# )
#
# Using a source directory:
qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../Packages ${TS_FILES})


add_executable(${PROJECT_NAME}
    images/BrickMoney.rc
    images/images.qrc
    translations/translations.qrc
    ${SRC_FILES}
    ${QM_FILES}
)

message(STATUS "${THIRDPARTY_DIR}/KDDockWidgets/1.1.95/cl15-x64/lib/cmake/KDDockWidgets")
set(KDDockWidgets_DIR "${THIRDPARTY_DIR}/KDDockWidgets/1.1.95/cl15-x64/lib/cmake/KDDockWidgets")
find_package(KDDockWidgets REQUIRED)



target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql
    BrickMoney::Logging BrickMoney::BrickMoneyBackend KDAB::kddockwidgets)

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

set( FILES_TO_COPY
   ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/bin/log4cplusU.dll
   ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/bin/log4cplusUD.dll
   ${THIRDPARTY_DIR}/KDDockWidgets/1.1.95/cl15-x64/bin/kddockwidgets_multisplitter1.dll
   ${THIRDPARTY_DIR}/KDDockWidgets/1.1.95/cl15-x64/bin/kddockwidgets_multisplitter1d.dll
   ${THIRDPARTY_DIR}/KDDockWidgets/1.1.95/cl15-x64/bin/kddockwidgets1.dll
   ${THIRDPARTY_DIR}/KDDockWidgets/1.1.95/cl15-x64/bin/kddockwidgets1d.dll
)

foreach( file_i ${FILES_TO_COPY})
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


