#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RVO3D::RVO3D" for configuration "Release"
set_property(TARGET RVO3D::RVO3D APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(RVO3D::RVO3D PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libRVO3D.so.1.1.0"
  IMPORTED_SONAME_RELEASE "libRVO3D.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS RVO3D::RVO3D )
list(APPEND _IMPORT_CHECK_FILES_FOR_RVO3D::RVO3D "${_IMPORT_PREFIX}/lib/libRVO3D.so.1.1.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
