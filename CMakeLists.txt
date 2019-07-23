cmake_minimum_required(VERSION 3.11...3.14)
# 3.11 support for FetchContent
# 3.14 support for Visual Studio 2019

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE STRING ""
	)
	message(STATUS "Uses VCPKG CMAKE_TOOLCHAIN_FILE")
endif()

project(VinlandVikings
	DESCRIPTION
		"A editor for the board game Vinland Vikings"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
	src/vin/canvas.cpp
	src/vin/canvas.h
	src/vin/hexagon.cpp
	src/vin/hexagon.h
	src/vin/hexagonbatch.cpp
	src/vin/hexagonbatch.h
	src/vin/hextilemap.cpp
	src/vin/hextilemap.h
	src/vin/heximage.cpp
	src/vin/heximage.h
	src/vin/hexsideskey.cpp
	src/vin/hexsideskey.h
	src/vin/imguiextra.cpp
	src/vin/imguiextra.h
	src/vin/logger.h
	src/vin/types.h
	src/vin/hexdata.cpp
	src/vin/hexdata.h
	src/vin/vinlandwindow.cpp
	src/vin/vinlandwindow.h
	src/vin/main.cpp
)

find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS On) 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VinlandVikings)

source_group(TREE
	${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES}
)

set(CppSdl2ImGui TRUE BOOL "Must use ImGui")

include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
	GIT_REPOSITORY
		https://github.com/mwthinker/CppSdl2.git
	#GIT_TAG
	#	origin/imgui
)
FetchContent_MakeAvailable(CppSdl2)

# Load data.
FetchContent_Declare(VinlandVikingsData
	GIT_REPOSITORY
		git@github.com:mwthinker/VinlandVikingsData.git
		#https://github.com/mwthinker/VinlandVikingsData.git
	GIT_TAG
		origin/VinlandVikingsData
)
FetchContent_MakeAvailable(VinlandVikingsData)
FetchContent_GetProperties(VinlandVikingsData
	SOURCE_DIR
		VinlandVikingsData_SOURCE_DIR
)

set(ExternalDependencies
	CppSdl2
)

if (CppSdl2Test)
	set(ExternalDependencieTests "CppSdl2Test")
endif ()

set_target_properties(
	${ExternalDependencies}
	${ExternalDependencieTests}
	
	PROPERTIES FOLDER
		ExternalDependencies
)

# Copy data to build folder.
file(COPY ${VinlandVikingsData_SOURCE_DIR}/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${VinlandVikingsData_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(VinlandVikings ${SOURCES})

if (MSVC)
	target_compile_definitions(VinlandVikings
		PUBLIC
			_CRT_SECURE_NO_WARNINGS
	)
endif ()

target_link_libraries(VinlandVikings
	PRIVATE
		nlohmann_json
		nlohmann_json::nlohmann_json
		Threads::Threads
		${ExternalDependencies}
)
