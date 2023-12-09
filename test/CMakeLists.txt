# Define the executable 'test-runner'
add_executable(test-runner
	runtime-tests.cpp
)

# Link the 'engine' library and SDL dependencies
target_link_libraries(test-runner
PRIVATE
	library-testing
	${library_dep_libs}
	Catch2::Catch2WithMain
)

# Set output directories for build targets
set_target_properties(test-runner PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/tests"
)
# extras: catch2 ctest integration.
# add catch2's ctest cmake module and register the tests defined by mdml-tests
# using catch_discover_tests
include(ctest)
include(catch)
catch_discover_tests(test-runner)

# bonus: add a custom ctest target to the build system, to run tests in the
#        correct directory
add_custom_target(ctest
	command ctest -c $configuration --test-dir ./tests --output-on-failure
)
add_dependencies(ctest
	test-runner
	#copy_assets
)

# vim: ts=2 sw=2 noet foldmethod=indent :
