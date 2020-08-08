
find_package(log4cplus HINTS ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/)

target_link_libraries(${PROJECT_NAME} PUBLIC log4cplus::log4cplusU)
target_include_directories(${PROJECT_NAME} PUBLIC log4cplus::log4cplusU)

message(STATUS "Log4cplus (${log4cplus_DIR}) used by ${CMAKE_CURRENT_LIST_DIR}")


