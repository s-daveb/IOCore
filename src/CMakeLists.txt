# Define the library 'engine'
add_library(IOCore OBJECT
	Application.cpp
	Exception.cpp
	debuginfo.cpp
)
set_target_properties(IOCore PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)

target_include_directories(IOCore PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Add additional link options for boost::stacktrace
if (USE_BOOST_STACKTRACE)
	target_link_options(IOCore INTERFACE -rdynamic)
endif()

set(LIBRARY_DEP_LIBS
	${STACKTRACE_DEP_LIBS}
	#	${NLOHMANN_JSON_LIBS}
)
target_link_libraries(IOCore PUBLIC ${LIBRARY_DEP_LIBS})

# Define build targets for 'engine', 'engine-static', and 'engine-test'
add_library(IOCoreShared SHARED )
add_library(IOCore::IOCoreShared ALIAS IOCoreShared )
set_target_properties(IOCoreShared PROPERTIES
	OUTPUT_NAME "IOCore"
	POSITION_INDEPENDENT_CODE ON
)

add_library(IOCoreStatic STATIC)
add_library(IOCore::IOCoreStatic ALIAS IOCoreStatic )
set_target_properties(IOCoreStatic PROPERTIES OUTPUT_NAME "IOCore")

target_link_libraries(IOCoreShared PRIVATE IOCore)
target_link_libraries(IOCoreStatic PRIVATE IOCore)


# vim: ts=2 sw=2 noet foldmethod=indent :
