set(TARGET_NAME TestMain)

add_executable(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DHX_STRING_CUSTOM)
add_definitions(-DHX_STRING_CSTRING_NULL_SUPPORT)
add_definitions(-DHX_CONTAINERS_CUSTOM)

add_definitions(-DHX_NOEXCEPT_ENABLED)

target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_NAME})

#平台配置
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_sources(${TARGET_NAME} PRIVATE ${RESOURCE_FILES})
else()
	target_link_libraries(${TARGET_NAME} PRIVATE atomic)
	target_link_libraries(${TARGET_NAME} PRIVATE pthread)
	target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

file(GLOB_RECURSE HEADER_FILES *.h *.hpp)
file(GLOB_RECURSE SOURCE_FILES *.c *.cpp)

target_sources(${TARGET_NAME} PRIVATE ${HEADER_FILES} ${SOURCE_FILES})
