#===============================================================================
# CMake minimum version
cmake_minimum_required(VERSION 3.10)
#===============================================================================



#===============================================================================
# C++ Standard used by Engine code
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#===============================================================================
#===============================================================================
# Configuration types
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)
#===============================================================================
#===============================================================================
# Platform types
if(MSVC)
	set(CMAKE_GENERATOR_PLATFORM x64)
endif()
#===============================================================================



#===============================================================================
# Global compiler options
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()
#===============================================================================
#===============================================================================
# Global linker options
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()
#===============================================================================



#===============================================================================
# Use solution folders feature
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#===============================================================================



#===============================================================================
# Solution name
project(Corvus CXX)
#===============================================================================
#===============================================================================
# Projects
add_subdirectory(Corvus)
add_subdirectory(Sandbox)
#===============================================================================

