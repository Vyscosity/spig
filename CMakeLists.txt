# This is free and unencumbered software released into the public domain. 

cmake_minimum_required( VERSION 3.0 )
project( bigg )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set( SDL_STATIC ON CACHE BOOL "" FORCE )
#set( SDL_SHARED OFF CACHE BOOL "" FORCE )
set(SDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL)

add_subdirectory( deps )



include( deps/bgfx.cmake/cmake/util/ConfigureDebugging.cmake )
add_library( bigg STATIC src/bigg.cpp include/bigg.hpp)#src/bigg_assets.h src/bigg_imgui.hpp src/bigg_shaders.hpp include/bigg.hpp )
target_link_libraries( bigg PUBLIC 
#SDL2
SDL2main
SDL2-static

bgfx  
#imgui
#glfw
glm 
)
#set(SDL2_INCLUDE_DIRS "${SDL_DIR}/include")
# Support both 32 and 64 bit builds
#set(SDL2_LIBRARIES "${SDL_DIR}/lib/x64/SDL2.lib;${SDL_DIR}/lib/x64/SDL2main.lib")
#string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

target_include_directories( bigg PUBLIC include "${SDL_DIR}/include")

#option( BIGG_EXAMPLES "Build examples." ON )
#if( BIGG_EXAMPLES )
	add_subdirectory( examples )
#endif()

#option( BIGG_ASSET_GEN "Generate binary assets." OFF )
#if( BIGG_ASSET_GEN )
#	add_subdirectory( assets/gen )
#endif()

file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR}/examples/cubes)





# Specify the source directory path
#set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/textures)

# Specify the destination directory path in the build directory
#set(DESTINATION_DIR ${CMAKE_BINARY_DIR}/examples/cubes)

# Copy the entire Data folder to the build directory
#file(COPY ${SOURCE_DIR} DESTINATION ${DESTINATION_DIR} FILES_MATCHING PATTERN "*")