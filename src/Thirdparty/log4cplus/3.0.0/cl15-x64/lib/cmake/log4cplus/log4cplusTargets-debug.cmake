#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "log4cplus::log4cplusU" for configuration "Debug"
set_property(TARGET log4cplus::log4cplusU APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(log4cplus::log4cplusU PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/log4cplusUD.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/log4cplusUD.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS log4cplus::log4cplusU )
list(APPEND _IMPORT_CHECK_FILES_FOR_log4cplus::log4cplusU "${_IMPORT_PREFIX}/lib/log4cplusUD.lib" "${_IMPORT_PREFIX}/bin/log4cplusUD.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
