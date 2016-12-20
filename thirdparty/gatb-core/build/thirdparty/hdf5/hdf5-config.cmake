#-----------------------------------------------------------------------------
# HDF5 Config file for compiling against hdf5 build directory
#-----------------------------------------------------------------------------
GET_FILENAME_COMPONENT (SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

#-----------------------------------------------------------------------------
# User Options
#-----------------------------------------------------------------------------
SET (HDF5_ENABLE_PARALLEL OFF)
SET (HDF5_BUILD_FORTRAN   )
SET (HDF5_ENABLE_F2003    )
SET (HDF5_BUILD_CPP_LIB   )
SET (HDF5_BUILD_TOOLS     ON)
SET (HDF5_BUILD_HL_LIB    )
SET (HDF5_ENABLE_Z_LIB_SUPPORT ON)
SET (HDF5_ENABLE_SZIP_SUPPORT  OFF)
SET (HDF5_ENABLE_SZIP_ENCODING )
SET (HDF5_BUILD_SHARED_LIBS    OFF)

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
SET (HDF5_INCLUDE_DIR "/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/src;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/c++;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/hl;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/thirdparty/hdf5/tools;/MMCI/MS/DeNovoAssembly2/work/ReREAD/Sample_ORNA/ORNA/thirdparty/gatb-core/build/thirdparty/hdf5" "${HDF5_MPI_C_INCLUDE_PATH}" )

IF (HDF5_BUILD_FORTRAN)
  SET (HDF5_INCLUDE_DIR_FORTRAN "" )
ENDIF (HDF5_BUILD_FORTRAN)
  
IF (HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_CPP ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_HL_LIB)
  SET (HDF5_INCLUDE_DIR_HL ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_HL_LIB)

IF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_HL_CPP ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_TOOLS)
  SET (HDF5_INCLUDE_DIR_TOOLS ${HDF5_INCLUDE_DIR} )
ENDIF (HDF5_BUILD_TOOLS)

IF (HDF5_BUILD_SHARED_LIBS)
  SET (H5_BUILT_AS_DYNAMIC_LIB 1 )
ELSE (HDF5_BUILD_SHARED_LIBS)
  SET (H5_BUILT_AS_STATIC_LIB 1 )
ENDIF (HDF5_BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Version Strings
#-----------------------------------------------------------------------------
SET (HDF5_VERSION_STRING 1.8.11)
SET (HDF5_VERSION_MAJOR  1.8)
SET (HDF5_VERSION_MINOR  11)

#-----------------------------------------------------------------------------
# Don't include targets if this file is being picked up by another
# project which has already build hdf5 as a subproject
#-----------------------------------------------------------------------------
IF (NOT TARGET "hdf5")
  INCLUDE (${SELF_DIR}/hdf5-targets.cmake)
  SET (HDF5_LIBRARIES "hdf5;hdf5_tools")
ENDIF (NOT TARGET "hdf5")
