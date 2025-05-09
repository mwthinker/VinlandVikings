cmake_minimum_required(VERSION 3.24)
project(VinlandVikings
	DESCRIPTION
		"A editor for the board game Vinland Vikings"
	VERSION
		0.2.1
	LANGUAGES
		CXX
)

set(SOURCES
	src/main.cpp

	ExternalFetchContent.cmake
	CPack.cmake
	vcpkg.json
	CMakePresets.json
)

if (MSVC)
	configure_file(properties.rc ${CMAKE_CURRENT_BINARY_DIR}/properties.rc)
endif ()

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

include(ExternalFetchContent.cmake)

find_package(Threads REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(lyra CONFIG REQUIRED)
find_package(CppSdl2 CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS On) 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VinlandVikings)

source_group(TREE
	${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES}
)

# Copy data to build folder.
if (VinlandVikingsData)
	file(COPY ${VinlandVikingsData_SOURCE_DIR}/imageTiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${VinlandVikingsData_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${VinlandVikingsData_SOURCE_DIR}/installer DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${CMAKE_CURRENT_BINARY_DIR}/installer/icon.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif ()

if (MSVC)
	if (VinlandVikingsData)
		add_executable(VinlandVikings
			${SOURCES}
			${CMAKE_CURRENT_BINARY_DIR}/properties.rc
			${CMAKE_CURRENT_SOURCE_DIR}/icon.rc
		)
	else ()
		add_executable(VinlandVikings
			${SOURCES}
			${CMAKE_CURRENT_BINARY_DIR}/properties.rc
		)
	endif ()
else ()
	add_executable(VinlandVikings
		${SOURCES}
	)
endif ()

extract_git_hash()
create_guid()

message(STATUS "GIT_HASH: ${GIT_HASH}")
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")
message(STATUS "GUID: ${GUID}")

add_subdirectory(VinlandVikings_Lib)
add_subdirectory(VinlandVikings_Test)

target_link_libraries(VinlandVikings
	PRIVATE
		VinlandVikings::VinlandVikings_Lib
)

set_target_properties(VinlandVikings
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CPack.cmake)
