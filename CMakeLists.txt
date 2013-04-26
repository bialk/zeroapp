# Top-Level CmakeLists.txt
cmake_minimum_required(VERSION 2.8)

PROJECT( zeroapp )
# The version number.

#version MAJOR.MINOR.PATCH
set (zeroapp_VERSION_MAJOR 0)
set (zeroapp_VERSION_MINOR 1)
set (zeroapp_PATCH_VERSION 2)

set (zeroapp_VERSION
"${zeroapp_VERSION_MAJOR}.${zeroapp_VERSION_MINOR}.${zeroapp_PATCH_VERSION}")


set (zeroapp_SHAREDIR ${CMAKE_ROOT}/bialk/zeroapp)


# configure a header file to pass some of the CMake settings
# to the source code
CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/configzeroapp.h.in"
  "${PROJECT_BINARY_DIR}/configzeroapp.h"
  )

# add the binary tree to the search path for include files
# so that we will find configzeroapp.h
include_directories("${PROJECT_BINARY_DIR}")

#SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_BUILD_TYPE Debug )

ADD_SUBDIRECTORY( src )
