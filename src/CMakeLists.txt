# Define the library 'engine'
add_library(library OBJECT
	library.cpp
)

# Define build targets for 'engine', 'engine-static', and 'engine-test'
add_library(library SHARED $<TARGET_OBJECTS:engineobj>)
add_library(library-static STATIC $<TARGET_OBJECTS:engineobj>)
add_library(library-testing $<TARGET_OBJECTS:engineobj>)


# Link the SDL dependencies
target_link_libraries(library PUBLIC ${library_dep_libs} )
target_link_libraries(library-static PUBLIC ${library_dep_libs} )
target_link_libraries(library-testing PUBLIC ${library_dep_libs})
