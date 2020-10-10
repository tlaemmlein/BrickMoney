#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "KDAB::kddockwidgets_multisplitter" for configuration "Debug"
set_property(TARGET KDAB::kddockwidgets_multisplitter APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(KDAB::kddockwidgets_multisplitter PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/kddockwidgets_multisplitter1d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/kddockwidgets_multisplitter1d.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS KDAB::kddockwidgets_multisplitter )
list(APPEND _IMPORT_CHECK_FILES_FOR_KDAB::kddockwidgets_multisplitter "${_IMPORT_PREFIX}/lib/kddockwidgets_multisplitter1d.lib" "${_IMPORT_PREFIX}/bin/kddockwidgets_multisplitter1d.dll" )

# Import target "KDAB::kddockwidgets" for configuration "Debug"
set_property(TARGET KDAB::kddockwidgets APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(KDAB::kddockwidgets PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/kddockwidgets1d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/kddockwidgets1d.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS KDAB::kddockwidgets )
list(APPEND _IMPORT_CHECK_FILES_FOR_KDAB::kddockwidgets "${_IMPORT_PREFIX}/lib/kddockwidgets1d.lib" "${_IMPORT_PREFIX}/bin/kddockwidgets1d.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
