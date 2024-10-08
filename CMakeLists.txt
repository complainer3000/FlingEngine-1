cmake_minimum_required ( VERSION 3.11.0 )
set( CMAKE_SUPPRESS_REGENERATION true )
set( CMAKE_INSTALL_MESSAGE LAZY )   # Skips update to date messages

project ("FlingEngine")

add_subdirectory(deps)

# Generated folder used for Cmake generated files
set( GENERATED_INC_FOLDER ${CMAKE_BINARY_DIR}/Generated )
message( STATUS "Generated dir: ${GENERATED_INC_FOLDER}" )

# Let us use our *.cmake files now
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
message( STATUS "Cmake mod path: " ${CMAKE_MODULE_PATH} )

# Include any .cmake macros here from the ./Cmake dir
include(FlingEngineInc)
include(MSVC_PCH)
include(FlingCompilerFlag)
include (InstallRequiredSystemLibraries)

set ( FLING_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )

# Option for the current build configuration. Note: going forward, we should use the FLING_Compiler_Flag macro
OPTION( DEFINE_SHIPPING "DEFINE_SHIPPING configuration will change asset paths to be relative." OFF )
OPTION( WITH_IMGUI_FLAG "WITH_IMGUI_FLAG will enable or disable the addition of IMGUI to the rendering pipeline. " ON )
OPTION( WITH_EDITOR_FLAG "Enables or disables the editor in the Fling Engine!" ON )
OPTION( ENABLE_MULTICORE "ENABLE_MULTICORE will allow MSVC to use all cores by adding the /MP option" ON)

# We can't have the editor without ImGUI!
IF( NOT WITH_IMGUI_FLAG AND WITH_EDITOR_FLAG )
    SET( WITH_EDITOR_FLAG OFF )
    message( FATAL_ERROR    "WITH_EDITOR_FLAG cannot be enabled because WITH_IMGUI_FLAG is OFF!" )
endif()

message( STATUS "----- Build Config ----" )
message( STATUS "Compiler=${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}" )
message( STATUS "WITH_EDITOR_FLAG=${WITH_EDITOR_FLAG}" )
message( STATUS "WITH_IMGUI_FLAG=${WITH_IMGUI_FLAG}" )
message( STATUS "DEFINE_SHIPPING=${DEFINE_SHIPPING}" )
message( STATUS "ENABLE_MULTICORE=${ENABLE_MULTICORE}" )
message( STATUS "FLING_ROOT_DIR=${FLING_ROOT_DIR}" )

# set the flags to 0 or 1 respectively for ImGUI and the Editor
IF( WITH_IMGUI_FLAG )
    ADD_DEFINITIONS ( -DWITH_IMGUI=1 )
else()
    ADD_DEFINITIONS ( -DWITH_IMGUI=0 )
endif()

IF( WITH_EDITOR_FLAG )
    ADD_DEFINITIONS ( -DWITH_EDITOR=1 )
else()
    ADD_DEFINITIONS ( -DWITH_EDITOR=0 )
endif()

IF( DEFINE_SHIPPING )
    message( STATUS "Build set to SHIPPING configuration!" )
    ADD_DEFINITIONS ( -DFLING_SHIPPING )
    SET( CMAKE_BUILD_TYPE Release )
ENDIF( DEFINE_SHIPPING )
message( STATUS "-----------------------" )

# C++17 standard
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Take care of warnings about strcpy
if( MSVC )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	if ( NOT DEFINED NOMINMAX )
		add_definitions( -DNOMINMAX )	# Entt warning, see https://github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-C4003-the-min-the-max-and-the-macro
    endif()
    
    # Enable multiple processor compilation
    if(ENABLE_MULTICORE)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()
# GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    add_definitions( -fPIC )

# Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions( -fPIC )
endif()

# This is needed for Clang support of glm::hash
ADD_DEFINITIONS( -DGLM_FORCE_CXX17 )

# SPIRV-Cross
add_definitions( -DSPIRV_CROSS_SHARED=OFF )
add_definitions( -DSPIRV_CROSS_STATIC=OFF )
add_definitions( -DSPIRV_CROSS_CLI=OFF )

set(SPIRV_CROSS_CLI OFF CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_MSL OFF CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_HLSL OFF CACHE BOOL "" FORCE)

# Make the folders look pretty in Visual Studio
set_target_properties(spirv-cross-c PROPERTIES FOLDER SPIRV-Cross)
set_target_properties(spirv-cross-core PROPERTIES FOLDER SPIRV-Cross)
set_target_properties(spirv-cross-cpp PROPERTIES FOLDER SPIRV-Cross)
set_target_properties(spirv-cross-glsl PROPERTIES FOLDER SPIRV-Cross)
set_target_properties(spirv-cross-reflect PROPERTIES FOLDER SPIRV-Cross)
set_target_properties(spirv-cross-util PROPERTIES FOLDER SPIRV-Cross)

# Add the engine and tests
add_subdirectory ( "FlingEngine" )
add_subdirectory ( "FlingTests" )

# Add a subdirectory that uses the FlingEngine and produces an executeable
add_subdirectory ( "Sandbox" )
