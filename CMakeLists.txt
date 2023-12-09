cmake_minimum_required(VERSION 3.12)
project(<APPLICATION-NAME>)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Initialize pkgconf
#find_package(PkgConfig REQUIRED)

# Example: Find SDL2, SDL2_image, and SDL2_gfx using PkgConfig
# add IMPORTED_TARGET to enable fancy PkgConfig::SDL2 syntax
#pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET SDL2)
#pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
#pkg_check_modules(SDL2_GFX REQUIRED IMPORTED_TARGET SDL2_gfx)

#set(engine_DEP_LIBS
#	PkgConfig::SDL2
#	PkgConfig::SDL2_IMAGE
#	PkgConfig::SDL2for_GFX
#	# if not using PkgConfig:: IMPORTED_TARGET syntax
#	# ${SDL2_LIBRARIES}
#	#${SDL2_IMG_LIBRARIES}
#	# ${SDL2_GFX_LIBRARIES}
#)

# FetchContent for Catch2
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)

