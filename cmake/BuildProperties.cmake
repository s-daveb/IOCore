# BuildProperties.cmake
# Copyright (c) 2024 Saul D Beniquez
# License: MIT
#
# This module defines a function prevent_in_source_build() that prevents in-source builds
# and sets a policy for CMake version 3.24.0 and above.

function(prevent_in_source_build)
	# Prevent in-source builds
	if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
		message(FATAL_ERROR "Source and build directories cannot be the same.")
	endif()
endfunction()

function(disable_deprecated_features)
	# Use new timestamp behavior when extracting files archives
	if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
	endif()
endfunction()

function(disable_tests_if_subproject)
if (NOT DEFINED PROJECT_NAME)
	option(ENABLE_TESTS "Build and run unit tests" ON)
else()
	option(ENABLE_TESTS "Build and run unit tests" OFF)
endif()
endfunction()


# vim: ts=4 sts=4 sw=4 noet :
