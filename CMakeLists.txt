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
	VERSION
		0.2.0
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") # 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
endif ()

set(SOURCES
	src/vin/hex/hash.h
	src/vin/hex/hex.h
	src/vin/hex/hexagon.cpp
	src/vin/hex/hexagon.h
	src/vin/hex/hexsides.h
	src/vin/hex/hexsideskey.h
	src/vin/hex/tileboard.cpp
	src/vin/hex/tileboard.h
	src/vin/hex/mapgenerator.cpp
	src/vin/hex/mapgenerator.h
	src/vin/hex/shape.cpp
	src/vin/hex/shape.h

	src/vin/batchmanager.cpp
	src/vin/batchmanager.h
	src/vin/camera.cpp
	src/vin/camera.h
	src/vin/graphic.cpp
	src/vin/graphic.h
	
	src/vin/action.cpp
	src/vin/action.h
	src/vin/hexagonbatch.cpp
	src/vin/hexagonbatch.h
	src/vin/hexdata.cpp
	src/vin/hexdata.h
	src/vin/hexdimension.h
	src/vin/heximage.cpp
	src/vin/heximage.h
	src/vin/imguiextra.cpp
	src/vin/imguiextra.h
	src/vin/hexcanvas.cpp
	src/vin/hexcanvas.h
	src/vin/logger.h
	src/vin/main.cpp
	src/vin/tilelexicon.cpp
	src/vin/tilelexicon.h
	src/vin/random.h
	src/vin/tilesgraphic.cpp
	src/vin/tilesgraphic.h
	src/vin/tile.h
	src/vin/tile.cpp
	src/vin/types.h
	src/vin/vinlandwindow.cpp
	src/vin/vinlandwindow.h
)

find_package(Threads REQUIRED)
find_package(fmt CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS On) 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VinlandVikings)

source_group(TREE
	${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES}
)

add_subdirectory(Config)

include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
	GIT_REPOSITORY
		https://github.com/mwthinker/CppSdl2.git
	GIT_TAG
		#origin/CppSdl2
		f5f946c39e3d26b580187d021ae0593cd60bda56
)
FetchContent_MakeAvailable(CppSdl2)

# Load data.
FetchContent_Declare(VinlandVikingsData
	GIT_REPOSITORY
		#git@github.com:mwthinker/VinlandVikingsData.git
		https://github.com/mwthinker/VinlandVikingsData.git
	GIT_TAG
		#origin/VinlandVikingsData
		e2dfd5decc6daf8555ec02dbfa43dd873d67765e
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
file(COPY ${VinlandVikingsData_SOURCE_DIR}/installer DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(VinlandVikings ${SOURCES})

if (WIN32)
	execute_process(COMMAND cmd /C "git -C ${CMAKE_CURRENT_SOURCE_DIR} rev-parse HEAD" OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
else ()
	execute_process(COMMAND bash -c "git -C ${CMAKE_CURRENT_SOURCE_DIR} rev-parse HEAD" OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
endif ()
message(STATUS "GIT_HASH: ${GIT_HASH}")
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")

target_compile_definitions(VinlandVikings
	PUBLIC
		GIT_VERSION="${GIT_HASH}"
		PROJECT_VERSION="${PROJECT_VERSION}"
)

option(ImGuiDemoWindow "Add ImGui Demo Window" OFF)
if (ImGuiDemoWindow)
	message(STATUS "ImGui Demo Window: Activated")
	target_compile_definitions(VinlandVikings
		PUBLIC
			IMGUI_DEMO_WINDOW
	)
endif ()

if (MSVC)
	target_compile_definitions(VinlandVikings
		PUBLIC
			_CRT_SECURE_NO_WARNINGS
	)
	
	message(STATUS "Hide terminal: -DHideTerminal=1")
	option(HideTerminal "Hide terminal" OFF)
	set_target_properties(VinlandVikings PROPERTIES WIN32_EXECUTABLE ${HideTerminal})

	target_sources(VinlandVikings
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/icon.rc
	)
endif ()

target_link_libraries(VinlandVikings
	PRIVATE
		Threads::Threads
		${ExternalDependencies}
		Config
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CPack.cmake)
