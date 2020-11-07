cmake_minimum_required(VERSION 3.10)

project(BrickMoneyProject)

#enable_testing()

add_subdirectory(Applications/BrickMoney)
add_subdirectory(Applications/BrickMoneyDesktop)
add_subdirectory(Applications/BrickMoneyManager)
add_subdirectory(Packages/BrickMoneyBackend)
add_subdirectory(Packages/Logging)
