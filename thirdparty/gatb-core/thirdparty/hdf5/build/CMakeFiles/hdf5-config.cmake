#-----------------------------------------------------------------------------
# HDF5 Config file for compiling against hdf5 install directory
#-----------------------------------------------------------------------------
GET_FILENAME_COMPONENT (SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${SELF_DIR}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
IF (NOT WIN32)
  GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
ENDIF (NOT WIN32)

#-----------------------------------------------------------------------------
# User Options
#-----------------------------------------------------------------------------
SET (HDF5_ENABLE_PARALLEL OFF)
SET (HDF5_BUILD_FORTRAN   )
SET (HDF5_ENABLE_F2003    )
SET (HDF5_BUILD_CPP_LIB   )
SET (HDF5_BUILD_TOOLS     OFF)
SET (HDF5_BUILD_HL_LIB    )
SET (HDF5_ENABLE_Z_LIB_SUPPORT OFF)
SET (HDF5_ENABLE_SZIP_SUPPORT  OFF)
SET (HDF5_ENABLE_SZIP_ENCODING )
SET (HDF5_BUILD_SHARED_LIBS    OFF)
SET (HDF5_PACKAGE_EXTLIBS      OFF)

#-----------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------
IF(HDF5_ENABLE_PARALLEL)
  SET(HDF5_MPI_C_INCLUDE_PATH "")
  SET(HDF5_MPI_C_LIBRARIES    "")
ENDIF(HDF5_ENABLE_PARALLEL)

#-----------------------------------------------------------------------------
# Directories
#-----------------------------------------------------------------------------
SET (HDF5_INCLUDE_DIR "${_IMPORT_PREFIX}/include" "${HDF5_MPI_C_INCLUDE_PATH}" )

IF (HDF5_BUILD_FORTRAN)
  SET (HDF5_INCLUDE_DIR_FORTRAN "${_IMPORT_PREFIX}/include/fortran" )
ENDIF (HDF5_BUILD_FORTRAN)
  
IF (HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_CPP "${_IMPORT_PREFIX}/include/cpp" )
ENDIF (HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_HL_LIB)
  SET (HDF5_INCLUDE_DIR_HL "${_IMPORT_PREFIX}/include/hl" )
ENDIF (HDF5_BUILD_HL_LIB)

IF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_HL_CPP "${_IMPORT_PREFIX}/include/hl/cpp" )
ENDIF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_TOOLS)
  SET (HDF5_INCLUDE_DIR_TOOLS "${_IMPORT_PREFIX}/include" )
  SET (HDF5_TOOLS_DIR "${_IMPORT_PREFIX}/bin" )
ENDIF (HDF5_BUILD_TOOLS)

#-----------------------------------------------------------------------------
# Version Strings
#-----------------------------------------------------------------------------
SET (HDF5_VERSION_STRING 1.8.11)
SET (HDF5_VERSION_MAJOR  1.8)
SET (HDF5_VERSION_MINOR  11)

#-----------------------------------------------------------------------------
# Don't include targets if this file is being picked up by another
# project which has already built hdf5 as a subproject
#-----------------------------------------------------------------------------
IF (NOT TARGET "hdf5")
  IF (HDF5_ENABLE_Z_LIB_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "zlib")
    INCLUDE (${SELF_DIR}/../ZLIB/-targets.cmake)
  ENDIF (HDF5_ENABLE_Z_LIB_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "zlib")
  IF (HDF5_ENABLE_SZIP_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "szip")
    INCLUDE (${SELF_DIR}/../SZIP/-targets.cmake)
  ENDIF (HDF5_ENABLE_SZIP_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "szip")
  INCLUDE (${SELF_DIR}/hdf5-targets.cmake)
  SET (HDF5_LIBRARIES "hdf5")
ENDIF (NOT TARGET "hdf5")

