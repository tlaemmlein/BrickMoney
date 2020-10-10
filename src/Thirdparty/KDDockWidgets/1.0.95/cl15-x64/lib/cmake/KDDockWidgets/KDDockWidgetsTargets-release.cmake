#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "KDAB::kddockwidgets_multisplitter" for configuration "Release"
set_property(TARGET KDAB::kddockwidgets_multisplitter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(KDAB::kddockwidgets_multisplitter PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/kddockwidgets_multisplitter1.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/kddockwidgets_multisplitter1.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS KDAB::kddockwidgets_multisplitter )
list(APPEND _IMPORT_CHECK_FILES_FOR_KDAB::kddockwidgets_multisplitter "${_IMPORT_PREFIX}/lib/kddockwidgets_multisplitter1.lib" "${_IMPORT_PREFIX}/bin/kddockwidgets_multisplitter1.dll" )

# Import target "KDAB::kddockwidgets" for configuration "Release"
set_property(TARGET KDAB::kddockwidgets APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(KDAB::kddockwidgets PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/kddockwidgets1.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/kddockwidgets1.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS KDAB::kddockwidgets )
list(APPEND _IMPORT_CHECK_FILES_FOR_KDAB::kddockwidgets "${_IMPORT_PREFIX}/lib/kddockwidgets1.lib" "${_IMPORT_PREFIX}/bin/kddockwidgets1.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
