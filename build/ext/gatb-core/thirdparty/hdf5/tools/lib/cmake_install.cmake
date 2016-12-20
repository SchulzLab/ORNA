# Install script for directory: /MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "toolsheaders")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5/h5trav.h;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5/h5tools.h;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5/h5tools_dump.h;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5/h5tools_utils.h;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5/h5tools_str.h;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5/h5tools_ref.h;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5/h5diff.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/build/ext/gatb-core/include/hdf5" TYPE FILE FILES
    "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5trav.h"
    "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools.h"
    "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_dump.h"
    "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_utils.h"
    "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_str.h"
    "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_ref.h"
    "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5diff.h"
    )
endif()

