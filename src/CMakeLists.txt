cmake_minimum_required(VERSION 3.10)

project(BrickMoneyProject)

#enable_testing()

add_subdirectory(Applications/BrickMoney)
add_subdirectory(Packages/Logging)
add_subdirectory(Packages/BrickMoneyData)
add_subdirectory(Packages/LegoSetInfoGenerator)
add_subdirectory(Packages/LegoSetModels)