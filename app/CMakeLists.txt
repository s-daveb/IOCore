
add_executable(app main.cpp)

#target_include_directories(app PRIVATE
#	${CMAKE_SOURCE_DIR}/src/
#)

target_link_libraries(app PRIVATE
	library	
)

# Manually adds a predefined target as a dependency
#add_dependencies(app copy-assets)

# vim: ts=2 sw=2 noet foldmethod=indent :
