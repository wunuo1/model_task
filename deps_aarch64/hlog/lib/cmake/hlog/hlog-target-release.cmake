#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bole::hlog" for configuration "Release"
set_property(TARGET bole::hlog APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bole::hlog PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libhlog.so.1.13.4"
  IMPORTED_SONAME_RELEASE "libhlog.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS bole::hlog )
list(APPEND _IMPORT_CHECK_FILES_FOR_bole::hlog "${_IMPORT_PREFIX}/lib/libhlog.so.1.13.4" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
