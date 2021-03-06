cmake_minimum_required(VERSION 3.1)

project(BrickMoneyWidget LANGUAGES CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(${PROJECT_NAME}
 Images/Images.qrc
 main.cpp
 MainWindow.cpp MainWindow.h MainWindow.ui
 LegoSetModel.cpp LegoSetModel.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${_qt5Core_install_prefix}/bin/windeployqt.exe
  --verbose 0
  --force
  --no-compiler-runtime
  --qmldir ${_qt5Core_install_prefix}/qml
   $<TARGET_FILE:${PROJECT_NAME}>)

