#===============================================================================
# Project
set(PROJECT_NAME Sandbox)
#===============================================================================



#===============================================================================
# Project General files
set(GENERAL_FILES_PATH "Source")
set(PROJECT_GENERAL_FILES
    "${GENERAL_FILES_PATH}/Application.cpp"
)
source_group("${GENERAL_FILES_PATH}" FILES ${PROJECT_GENERAL_FILES})
#===============================================================================
#===============================================================================
# Project files combined
set(PROJECT_ALL_FILES
    ${PROJECT_GENERAL_FILES}
)
#===============================================================================



#===============================================================================
# Target executable
add_executable(${PROJECT_NAME} ${PROJECT_ALL_FILES})
#===============================================================================



#===============================================================================
# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/Corvus/Source;"
    "${CMAKE_SOURCE_DIR}/Corvus/Dependencies/spdlog/include"
)
#===============================================================================
#===============================================================================
# Output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   		"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG   		"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   		"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    PDB_OUTPUT_DIRECTORY_DEBUG   			"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG   	"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
	
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE 		"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE 		"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE 		"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    PDB_OUTPUT_DIRECTORY_RELEASE 			"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE 	"${CMAKE_SOURCE_DIR}/Binary/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
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
		
		# Corvus specific definitions
		"CORVUS_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
		
		# Corvus specific definitions
		
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
        $<$<CONFIG:Debug>:
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG;
        /SUBSYSTEM:CONSOLE
    )
endif()
#===============================================================================



#===============================================================================
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Corvus
)
#===============================================================================

