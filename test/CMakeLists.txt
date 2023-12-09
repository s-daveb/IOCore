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
