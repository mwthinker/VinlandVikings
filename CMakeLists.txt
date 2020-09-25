cmake_minimum_required(VERSION 3.16)
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
set(CMAKE_CXX_EXTENSIONS ON)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") # 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
endif ()

set(SOURCES
	src/vin/hex/hash.h
	src/vin/hex/hex.h
	src/vin/hex/hexagon.cpp
	src/vin/hex/hexagon.h
	src/vin/hex/tile.h
	src/vin/hex/tilekey.h
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
	src/vin/logger.cpp
	src/vin/logger.h
	src/vin/main.cpp
	src/vin/tilelexicon.cpp
	src/vin/tilelexicon.h
	src/vin/random.h
	src/vin/tilesgraphic.cpp
	src/vin/tilesgraphic.h
	src/vin/spritetile.h
	src/vin/spritetile.cpp
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

include(ExternalFetchContent.cmake)

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
file(COPY ${VinlandVikingsData_SOURCE_DIR}/imageTiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${VinlandVikingsData_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${VinlandVikingsData_SOURCE_DIR}/installer DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(VinlandVikings ${SOURCES})

extract_git_hash()
create_guid()

message(STATUS "GIT_HASH: ${GIT_HASH}")
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")
message(STATUS "GUID: ${GUID}")

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

	if (MSVC)
		target_compile_options(VinlandVikings
			PUBLIC
				"/permissive-"
				"/MP"
		)
	endif (MSVC)
	
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
