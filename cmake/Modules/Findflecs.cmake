# modified FindGLFW3.cmake to work with flecs

set( _flecs_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/includes"
"C:/Program Files (x86)/flecs/include" )
set( _flecs_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/lib"
"C:/Program Files (x86)/flecs/lib" )

# Check environment for root search directory
set( _flecs_ENV_ROOT $ENV{flecs_ROOT} )
if( NOT flecs_ROOT AND _flecs_ENV_ROOT )
	set(flecs_ROOT ${_flecs_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( flecs_ROOT )
	list( INSERT _flecs_HEADER_SEARCH_DIRS 0 "${flecs_ROOT}/include" )
	list( INSERT _flecs_LIB_SEARCH_DIRS 0 "${flecs_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(flecs_INCLUDE_DIRS "flecs.h"
PATHS ${_flecs_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(flecs_LIBRARY NAMES flecs flecs_static
PATHS ${_flecs_LIB_SEARCH_DIRS} )
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(flecs DEFAULT_MSG
flecs_LIBRARY flecs_INCLUDE_DIRS)
