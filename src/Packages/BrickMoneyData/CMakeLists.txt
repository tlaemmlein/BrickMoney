
project(BrickMoneyData)

set(SRC_FILES
    LegoSet.cpp
    LegoSet.h
    BrickMoneyDataManager.cpp
    BrickMoneyDataManager.h
    LegoSetDataSource.cpp
    LegoSetDataSource.h
    magic_enum.h
)

find_package(Qt5 COMPONENTS Core REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(${PROJECT_NAME} STATIC ${SRC_FILES} )
add_library(BrickMoney::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core BrickMoney::Logging)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(${CMAKE_CURRENT_SOURCE_DIR}/../Logging/Logging.cmake)


message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")




