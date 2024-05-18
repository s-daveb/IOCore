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

package_library_headers(IOCore
	${IOCore_CMAKE_SOURCE_DIR}/include
)

target_include_directories(IOCore PRIVATE ${IOCore_CMAKE_SOURCE_DIR}/include)
target_include_directories(IOCore INTERFACE ${IOCore_INCLUDE_OUTPUT_DIR})

# Add additional link options for boost::stacktrace
if (USE_BOOST_STACKTRACE)
	target_link_options(IOCore PUBLIC -rdynamic)
endif()

set(LIBRARY_DEP_LIBS
	${STACKTRACE_DEP_LIBS}
)

target_link_libraries(IOCore PUBLIC ${LIBRARY_DEP_LIBS})
target_link_libraries(IOCore PUBLIC fmt::fmt-header-only)

add_library(IOCoreStatic STATIC)
add_library(IOCore::Static ALIAS IOCoreStatic )
set_target_properties(IOCoreStatic PROPERTIES OUTPUT_NAME "IOCore")

add_library(IOCoreShared SHARED )
add_library(IOCore::Shared ALIAS IOCoreShared )
set_target_properties(IOCoreShared PROPERTIES OUTPUT_NAME "IOCore")

target_link_libraries(IOCoreStatic PRIVATE IOCore)
target_link_libraries(IOCoreShared PRIVATE IOCore)

target_link_libraries(IOCoreStatic PUBLIC fmt::fmt-header-only)
target_link_libraries(IOCoreShared PUBLIC fmt::fmt-header-only)

target_include_directories(IOCoreStatic INTERFACE ${IOCore_INCLUDE_OUTPUT_DIR})
target_include_directories(IOCoreShared INTERFACE ${IOCore_INCLUDE_OUTPUT_DIR})



# vim: ts=2 sw=2 noet foldmethod=indent :
