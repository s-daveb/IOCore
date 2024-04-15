# Define the library 'engine'
add_library(IOCore OBJECT
	Application.cpp
	Exception.cpp
	FileResource.cpp
	debuginfo.cpp
)
set_target_properties(IOCore PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)
target_include_directories(IOCore PUBLIC ${IOCore_INCLUDE_DIRS})

# Add additional link options for boost::stacktrace
if (USE_BOOST_STACKTRACE)
	target_link_options(IOCore PUBLIC -rdynamic)
endif()

set(LIBRARY_DEP_LIBS
	${STACKTRACE_DEP_LIBS}
	fmt::fmt-header-only
)
target_link_libraries(IOCore PUBLIC ${LIBRARY_DEP_LIBS})

add_library(IOCoreStatic STATIC)
add_library(IOCore::IOCoreStatic ALIAS IOCoreStatic )
set_target_properties(IOCoreStatic PROPERTIES OUTPUT_NAME "IOCore")
target_link_libraries(IOCoreStatic PUBLIC IOCore)

add_library(IOCoreShared SHARED )
add_library(IOCore::IOCoreShared ALIAS IOCoreShared )
set_target_properties(IOCoreShared PROPERTIES OUTPUT_NAME "IOCore")
target_link_libraries(IOCoreShared PUBLIC IOCore)

target_include_directories(IOCoreStatic INTERFACE ${IOCore_INCLUDE_DIRS})
target_include_directories(IOCoreShared INTERFACE ${IOCore_INCLUDE_DIRS})

# vim: ts=2 sw=2 noet foldmethod=indent :
