#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "nmea_msgs::nmea_msgs__rosidl_typesupport_cpp" for configuration "Release"
set_property(TARGET nmea_msgs::nmea_msgs__rosidl_typesupport_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nmea_msgs::nmea_msgs__rosidl_typesupport_cpp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libnmea_msgs__rosidl_typesupport_cpp.so"
  IMPORTED_SONAME_RELEASE "libnmea_msgs__rosidl_typesupport_cpp.so"
  )

list(APPEND _cmake_import_check_targets nmea_msgs::nmea_msgs__rosidl_typesupport_cpp )
list(APPEND _cmake_import_check_files_for_nmea_msgs::nmea_msgs__rosidl_typesupport_cpp "${_IMPORT_PREFIX}/lib/libnmea_msgs__rosidl_typesupport_cpp.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
