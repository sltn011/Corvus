#===============================================================================
# Project
set(PROJECT_NAME Corvus)
#===============================================================================



#===============================================================================
# Project General files
set(GENERAL_FILES_PATH "Source")
set(PROJECT_GENERAL_FILES
    "${GENERAL_FILES_PATH}/Corvus.h"
)
source_group("${GENERAL_FILES_PATH}" FILES ${PROJECT_GENERAL_FILES})
#===============================================================================
#===============================================================================
# Project Core files
set(CORE_FILES_PATH "${GENERAL_FILES_PATH}/Corvus/Core")
set(PROJECT_CORE_FILES
    "${CORE_FILES_PATH}/Base.h"
    "${CORE_FILES_PATH}/PlatformDetection.h"
    "${CORE_FILES_PATH}/EntryPoint.h"
    "${CORE_FILES_PATH}/Application.h"
    "${CORE_FILES_PATH}/Application.cpp"
    "${CORE_FILES_PATH}/Log.h"
    "${CORE_FILES_PATH}/Log.cpp"
)
source_group("${CORE_FILES_PATH}" FILES ${PROJECT_CORE_FILES})
#===============================================================================
#===============================================================================
# Project files combined
set(PROJECT_ALL_FILES
    ${PROJECT_GENERAL_FILES}
    ${PROJECT_CORE_FILES}
)
#===============================================================================



#===============================================================================
# Target library
add_library(${PROJECT_NAME} STATIC ${PROJECT_ALL_FILES})
#===============================================================================



#===============================================================================
# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/spdlog/include"
)
#===============================================================================
#===============================================================================
# Output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
)
#===============================================================================



#===============================================================================
# Enable interpocedural optimization for Release builds
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
#===============================================================================



#===============================================================================
# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)
#===============================================================================
#===============================================================================
# Compile and link options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
    )
endif()
#===============================================================================

