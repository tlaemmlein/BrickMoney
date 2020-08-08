
find_package(log4cplus HINTS ${THIRDPARTY_DIR}/log4cplus/3.0.0/cl15-x64/)

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
    ARGS -E copy ${file_i} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
endforeach( file_i )

message(STATUS "Log4cplus (${log4cplus_DIR}) used by ${CMAKE_CURRENT_LIST_DIR}")


