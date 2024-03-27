# Define the executable 'test-runner'
add_executable(test-runner
	runtime-tests.cpp
	Application.test.cpp
	Exception.test.cpp
)

target_include_directories(test-runner PRIVATE
	${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(test-runner
PRIVATE
	IOCoreStatic
	Catch2::Catch2WithMain
)

# Set output directories for build targets
set_target_properties(test-runner PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)

# extras: catch2 ctest integration.
# add catch2's ctest cmake module and register the tests defined by mdml-tests
# using catch_discover_tests
include(CTest)
include(Catch)
catch_discover_tests(test-runner)

# bonus: add a custom ctest target to the build system, to run tests in the
#        correct directory
add_custom_target(ctest
	command ctest -c $configuration --test-dir . --output-on-failure
)
add_dependencies(ctest
	test-runner
	#copy_assets
)

# vim: ts=2 sw=2 noet foldmethod=indent :
