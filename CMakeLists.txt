cmake_minimum_required(VERSION 3.24)

project(VinlandVikings
	DESCRIPTION
		"A editor for the board game Vinland Vikings"
	VERSION
		0.2.0
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

	src/vin/camera.cpp
	src/vin/camera.h
	src/vin/commandmanager.h
	src/vin/deck.h
	
	src/vin/action.cpp
	src/vin/action.h
	src/vin/configuration.cpp
	src/vin/configuration.h
	src/vin/hexagonbatch.cpp
	src/vin/hexagonbatch.h
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

	ExternalFetchContent.cmake
	CPack.cmake
	vcpkg.json
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
find_package(CppSdl2 REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS On) 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VinlandVikings)

source_group(TREE
	${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES}
)

set(ExternalDependencies
	CppSdl2
	ImGui
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
			NOMINMAX
			_CRT_SECURE_NO_WARNINGS
	)

	#  /permissive-
	target_compile_options(VinlandVikings
		PUBLIC
			/W3 /WX
			/MP
	)
	
	message(STATUS "Hide terminal: -DHideTerminal=1")
	option(HideTerminal "Hide terminal" OFF)
	set_target_properties(VinlandVikings PROPERTIES WIN32_EXECUTABLE ${HideTerminal})
else ()
	target_compile_options(VinlandVikings
		PRIVATE
			-Wall -pedantic -Wcast-align -Woverloaded-virtual -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function
			 -fconcepts-diagnostics-depth=2
	)
endif ()

target_link_libraries(VinlandVikings
	PRIVATE
		Threads::Threads
		${ExternalDependencies}
		nlohmann_json
		nlohmann_json::nlohmann_json
		fmt::fmt
		bfg::lyra
		CppSdl2
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CPack.cmake)
