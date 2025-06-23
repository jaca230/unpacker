cmake_minimum_required(VERSION 3.14)

project(unpacker VERSION 0.1 LANGUAGES CXX)

# -----------------------------------------------------------
# C++ standard and build options
# -----------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------
# Include helpers
# -----------------------------------------------------------
include(GNUInstallDirs)              # For standard install dirs (lib, include, etc)
include(CMakePackageConfigHelpers)  # For config file helpers

# -----------------------------------------------------------
# Require MIDASSYS environment variable
# -----------------------------------------------------------
if(NOT DEFINED ENV{MIDASSYS})
  message(FATAL_ERROR
    "Environment variable MIDASSYS not set.\n"
    "Please source your MIDAS environment or run the detect_environment.sh script."
  )
endif()

set(MIDASSYS_INCLUDE_DIRS
  "$ENV{MIDASSYS}/include"
  "$ENV{MIDASSYS}/midasio"
)
set(MIDASSYS_LIB_DIR "$ENV{MIDASSYS}/lib")

# -----------------------------------------------------------
# Find external dependencies (assumed installed on system)
# -----------------------------------------------------------
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Tree Hist)
find_package(nlohmann_json REQUIRED)
include(${ROOT_USE_FILE})

# -----------------------------------------------------------
# Source and header files
# -----------------------------------------------------------
file(GLOB_RECURSE COMMON_DATA_PRODUCTS_SRCS CONFIGURE_DEPENDS common/data_products/*.cc)
file(GLOB_RECURSE COMMON_UNPACKING_SRCS CONFIGURE_DEPENDS common/unpacking/*.cc)
file(GLOB_RECURSE NALU_DATA_PRODUCTS_SRCS CONFIGURE_DEPENDS nalu/data_products/*.cc)
file(GLOB_RECURSE NALU_UNPACKING_SRCS CONFIGURE_DEPENDS nalu/unpacking/*.cc)

file(GLOB_RECURSE COMMON_DATA_PRODUCTS_HEADERS CONFIGURE_DEPENDS include/common/data_products/*.hh)
file(GLOB_RECURSE COMMON_UNPACKING_HEADERS CONFIGURE_DEPENDS include/common/unpacking/*.hh)
file(GLOB_RECURSE NALU_DATA_PRODUCTS_HEADERS CONFIGURE_DEPENDS include/nalu/data_products/*.hh)
file(GLOB_RECURSE NALU_UNPACKING_HEADERS CONFIGURE_DEPENDS include/nalu/unpacking/*.hh)

# Consolidate all headers and include dirs
set(ALL_HEADERS
  ${COMMON_DATA_PRODUCTS_HEADERS}
  ${COMMON_UNPACKING_HEADERS}
  ${NALU_DATA_PRODUCTS_HEADERS}
  ${NALU_UNPACKING_HEADERS}
)

set(ALL_INCLUDE_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  ${MIDASSYS_INCLUDE_DIRS}
  ${ROOT_INCLUDE_DIRS}
)
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

# Prepare ROOT dictionary include options for generation
set(ROOT_DICTIONARY_INCLUDE_OPTIONS "")
foreach(dir IN LISTS ALL_INCLUDE_DIRS)
  list(APPEND ROOT_DICTIONARY_INCLUDE_OPTIONS "-I${dir}")
endforeach()

# -----------------------------------------------------------
# Helper function: generate ROOT dictionaries
# -----------------------------------------------------------
function(add_dictionary libname headers linkdef)
  ROOT_GENERATE_DICTIONARY(
    ${libname}_dict
    "${headers}"
    LINKDEF "${linkdef}"
    OPTIONS ${ROOT_DICTIONARY_INCLUDE_OPTIONS}
  )
endfunction()

# -----------------------------------------------------------
# Macro to add a library with dictionary and proper install/export
# -----------------------------------------------------------
macro(add_unpacker_lib name sources headers linkdef deps)
  # Generate dictionary source
  add_dictionary(${name} "${headers}" "${linkdef}")

  # Add library, including dictionary source file
  add_library(${name} SHARED ${sources} ${CMAKE_CURRENT_BINARY_DIR}/${name}_dict.cxx)

  target_include_directories(${name}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
      ${MIDASSYS_INCLUDE_DIRS}
      ${ROOT_INCLUDE_DIRS}
  )

  target_link_libraries(${name} PUBLIC ${deps})

  # Add an ALIAS target for convenience and namespace usage
  add_library(unpacker::${name} ALIAS ${name})
endmacro()

# -----------------------------------------------------------
# Define your libraries here
# -----------------------------------------------------------
add_unpacker_lib(
  common_data_products
  "${COMMON_DATA_PRODUCTS_SRCS}"
  "${COMMON_DATA_PRODUCTS_HEADERS}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/common/data_products/LinkDef.h"
  "${ROOT_LIBRARIES}"
)

add_unpacker_lib(
  common_unpacking
  "${COMMON_UNPACKING_SRCS}"
  "${COMMON_UNPACKING_HEADERS}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/common/unpacking/LinkDef.h"
  "common_data_products;${ROOT_LIBRARIES};nlohmann_json::nlohmann_json;${MIDASSYS_LIB_DIR}/libmidas.a;${MIDASSYS_LIB_DIR}/libmfe.a"
)

add_unpacker_lib(
  nalu_data_products
  "${NALU_DATA_PRODUCTS_SRCS}"
  "${NALU_DATA_PRODUCTS_HEADERS}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/nalu/data_products/LinkDef.h"
  "common_data_products"
)

add_unpacker_lib(
  nalu_unpacking
  "${NALU_UNPACKING_SRCS}"
  "${NALU_UNPACKING_HEADERS}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/nalu/unpacking/LinkDef.h"
  "common_unpacking;nalu_data_products;${ROOT_LIBRARIES}"
)

# -----------------------------------------------------------
# Installation rules
# -----------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)

  # Install shared libraries and archive files
  install(TARGETS
    common_data_products
    common_unpacking
    nalu_data_products
    nalu_unpacking
    EXPORT unpackerTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  # Install header files preserving directory structure (no extra "unpacker" prefix)
  install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nalu
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hh"
  )
    # Install ROOT dictionary files by explicit paths, no checks at configure time
    foreach(lib IN ITEMS
    common_data_products
    common_unpacking
    nalu_data_products
    nalu_unpacking)

    set(dict_rootmap "${CMAKE_CURRENT_BINARY_DIR}/lib${lib}_dict.rootmap")
    set(dict_pcm "${CMAKE_CURRENT_BINARY_DIR}/lib${lib}_dict_rdict.pcm")

    install(FILES
        "${dict_rootmap}"
        "${dict_pcm}"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        OPTIONAL
    )
    endforeach()


  # Generate and install version file for config package
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  # Configure and install the Config.cmake file for find_package
  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unpacker
  )

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unpacker
  )

  # Export package for use with find_package (exported targets)
  export(PACKAGE unpacker)

endif()
