# Define the library 'engine'
add_library(IOCore OBJECT
	Application.cpp
	Exception.cpp
	debuginfo.cpp
)

set(LIBRARY_DEP_LIBS
	${STACKTRACE_DEP_LIBS}
	#	${NLOHMANN_JSON_LIBS}
)
target_link_libraries(IOCore PUBLIC ${LIBRARY_DEP_LIBS})

# Define build targets for 'engine', 'engine-static', and 'engine-test'
add_library(IOCoreShared SHARED $<TARGET_OBJECTS:IOCore>)
set_target_properties(IOCore PROPERTIES OUTPUT_NAME "IOCore")

add_library(IOCoreStatic STATIC $<TARGET_OBJECTS:IOCore>)
set_target_properties(IOCoreStatic PROPERTIES OUTPUT_NAME "IOCore")

# Link the SDL dependencies
#target_link_libraries(IOCoreStatic PUBLIC ${LIBRARY_DEP_LIBS})
#target_link_libraries(IOCoreShared PUBLIC ${LIBRARY_DEP_LIBS})

# vim: ts=2 sw=2 noet foldmethod=indent :
