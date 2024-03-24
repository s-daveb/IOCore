cmake_minimum_required(VERSION 3.26)

# Prevent in-source builds
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

# When this package is included as a subproject, there's no need to
# build and run the unit-tests. Setting -DENABLE_TESTS to false could
# speed up build time for pacakge maintainers
if (NOT DEFINED PROJECT_NAME)
	option(ENABLE_TESTS "Build and run unit tests" ON)
else()
	option(ENABLE_TESTS "Build and run unit tests" OFF)
endif()

option(USE_SYSTEM_CATCH2
	"Do not download & compile catch2 library for unit tests"
	ON
)


IF( NOT CMAKE_BUILD_TYPE )
  SET( CMAKE_BUILD_TYPE Debug )
ENDIF()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Disable GNU compiler extensions
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Search for the code caching compiler wrapper, ccache and enable it
# if found. This will speed up repeated builds.
find_program(CCACHE_PATH ccache)
if(CCACHE_PATH)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PATH})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PATH})
endif(CCACHE_PATH)

if (USE_SYSTEM_CATCH2)
	message(CHECK_START "Detecting System Catch2 ")
	find_package(Catch2 3 QUIET)

	if(TARGET Catch2::Catch2)
		message(CHECK_PASS "found target Catch2::Catch2")
	else()
		message(CHECK_FAIL "not found")
		set(USE_SYSTEM_CATCH2 OFF)
		message(STATUS "USE_SYSTEM_CATCH2=OFF")
	endif()
endif()


if (UNIX AND NOT(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux"))
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" OFF)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" ON)
else()
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" ON)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" OFF)
endif()
project(IOCore
	VERSION 0.1
	LANGUAGES C CXX
	# HOMEPAGE_URL <URL>
	DESCRIPTION "A framework for application management, file IO, etc"
)

# enable compile_commands.json generation for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

# Initialize FetchContent
include(FetchContent)

# Initialize CHECK_INCLUDE_FILES
include(CheckIncludeFile)

# Initialize pkgconf
find_package(PkgConfig REQUIRED)

# Example: Find SDL2, SDL2_image, and SDL2_gfx using PkgConfig
# add IMPORTED_TARGET to enable fancy PkgConfig::SDL2 syntax
#pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET SDL2)
#pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
#pkg_check_modules(SDL2_GFX REQUIRED IMPORTED_TARGET SDL2_gfx)

if (USE_EXECINFO_STACKTRACE AND (NOT USE_BOOST_STACKTRACE))
	CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)

	if (HAVE_EXECINFO_H)
		add_definitions(-DHAVE_EXECINFO_H=1)
	endif()

	find_library(LIB_EXEC_INFO
		NAMES execinfo  # Specify the library name without the 'lib' prefix or file extension
		HINTS /usr/lib /usr/local/lib  # Optional hint for the library location
	)
	if (LIB_EXEC_INFO)
		message(STATUS "Found libexecinfo: ${LIB_EXEC_INFO}")
		set(STACKTRACE_DEP_LIBS ${LIB_EXEC_INFO})
	endif()
elseif((NOT USE_EXECINFO_STACKTRACE) AND USE_BOOST_STACKTRACE)
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_STATIC_RUNTIME OFF)  # Do not require static C++ runtime
	set(Boost_USE_MULTITHREADED ON)
	find_package(Boost 1.82.0 COMPONENTS system filesystem REQUIRED)

	if (Boost_FOUND)
		add_definitions(-DBOOST_STACKTRACER=1)
		add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE=1)

		include_directories(${Boost_INCLUDE_DIRS})
	endif()
endif()

if (ENABLE_TESTS)
	if (NOT USE_SYSTEM_CATCH2)
		FetchContent_Declare(
    	  Catch2
	    	URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.zip
	    	URL_HASH MD5=c426e77d4ee0055410bc930182959ae5
		)
		FetchContent_MakeAvailable(Catch2)
	else()
		include_directories(${CATCH2_INCLUDE_DIRS})
		link_directories(${CATCH2_LIBRARY_DIRS})
	endif()
	FetchContent_Declare(
		FakeIt
		URL https://github.com/eranpeer/FakeIt/archive/refs/tags/2.4.0.zip
		URL_HASH MD5=72e4ce7f1c0de97074d2d5b517753286
		FIND_PACKAGE_ARGS
	)
	FetchContent_MakeAvailable(FakeIt)

	if (TARGET Catch2)
		set_target_properties(Catch2 PROPERTIES
			CXX_STANDARD 20
		)
	endif()

	if (TARGET Catch2WithMain)
		set_target_properties(Catch2WithMain PROPERTIES
 		       	CXX_STANDARD 20
		)
	endif()
	list(APPEND cmake_module_path ${Catch2_source_dir}/extras)
endif()

set(ARTIFACT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Set output directories for build targets
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARTIFACT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ARTIFACT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ARTIFACT_DIRECTORY}/bin)

# Include directories
#include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Example: Add a target to copy application resource files to the build dir
add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_BINARY_DIR}/tests/data
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${ARTIFACT_DIRECTORY}/share/data
)

# Add subdirectories
add_subdirectory(src)

if(ENABLE_TESTS)
	add_subdirectory(tests)
endif()


# vim: ts=2 sw=2 noet foldmethod=indent :
