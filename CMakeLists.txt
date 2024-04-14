cmake_minimum_required(VERSION 3.26)

SET(IOCore_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(IOCore_INCLUDE_DIRS ${IOCore_CMAKE_SOURCE_DIR}/include)

# Additional paths to search for custom and third-party CMake modules
list(APPEND CMAKE_MODULE_PATH ${IOCore_CMAKE_SOURCE_DIR}/cmake)

include(BuildProperties)

prevent_in_source_build()
disable_deprecated_features()

# When this package is included as a subproject, there's no need to
# build and run the unit-tests.
# Sets -DBUILD_TESTING to false by default if this is a third-party lib build
# This check must appear before project()
disable_tests_if_subproject()

project(IOCore
	VERSION 0.2.13
	LANGUAGES C CXX
	# HOMEPAGE_URL <URL>
	DESCRIPTION "A framework for application management, file IO, etc"
)

include(CPM)

option(USE_CCACHE
	[=[Use ccache compiler cache to speed up builds.
Enabled by default if ccache is found]=]
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
if (USE_CCACHE)
	message(CHECK_START "Detecting cacche")

	find_program(CCACHE_PATH ccache)
	if(CCACHE_PATH)
  	  message(CHECK_PASS("found"))
  	  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PATH})
  	  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PATH})
	endif()

	list(APPEND CMAKE_MESSAGE_INDENT " ")
  	message(STATUS "(set -DUSE_CCACHE=Off to disable)")
	list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()


if (UNIX AND NOT(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux"))
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" OFF)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" ON)
else()
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" ON)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" OFF)
endif()


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

if (BUILD_TESTING)
	CPMFindPackage(NAME Catch2
		GITHUB_REPOSITORY catchorg/Catch2
		VERSION 3.4.0
		OPTIONS
			"CATCH_DEVELOPMENT_BUILD OFF"
			"CATCH_BUILD_TESTING OFF"
	)
	CPMFindPackage(NAME FakeIt
		GITHUB_REPOSITORY eranpeer/FakeIt
		GIT_TAG 2.4.0
		OPTIONS
 	   		"BUILD_TESTINGING OFF"
	)

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

	if (Catch2_ADDED)
		list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
	else()
		if (Catch2_DIR)
			list(APPEND CMAKE_MODULE_PATH ${Catch2_DIR})
		endif()
	endif()
endif()

# Set output directories for build targets
set_artifact_dir(${CMAKE_BINARY_DIR}/out)

# Add subdirectories
add_subdirectory(src)

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()


# vim: ts=4 sw=4 noet foldmethod=indent :
