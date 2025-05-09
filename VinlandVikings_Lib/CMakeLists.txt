project(VinlandVikings_Lib
	DESCRIPTION
		"A lib for Vinland Vikings"
	LANGUAGES
		CXX
)

set(SOURCES_LIB
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
	src/vin/coordinatesystem.cpp
	src/vin/coordinatesystem.h
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
add_library(VinlandVikings_Lib ${SOURCES_LIB})
add_library(VinlandVikings::VinlandVikings_Lib ALIAS VinlandVikings_Lib)

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

find_package(Threads REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(lyra CONFIG REQUIRED)
find_package(CppSdl2 CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS On) 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VinlandVikings_Lib)

source_group(TREE
	${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES_LIB}
)

target_include_directories(VinlandVikings_Lib
	PUBLIC
		src/
)

target_compile_definitions(VinlandVikings_Lib
	PUBLIC
		GIT_VERSION="${GIT_HASH}"
		PROJECT_VERSION="${PROJECT_VERSION}"
)

option(ImGuiDemoWindow "Add ImGui Demo Window" OFF)
if (ImGuiDemoWindow)
	message(STATUS "ImGui Demo Window: Activated")
	target_compile_definitions(VinlandVikings_Lib
		PUBLIC
			IMGUI_DEMO_WINDOW
	)
endif ()

if (MSVC)
	message(STATUS "Hide terminal: -DHideTerminal=1")
	option(HideTerminal "Hide terminal" OFF)
	set_target_properties(VinlandVikings PROPERTIES WIN32_EXECUTABLE ${HideTerminal})
	target_compile_definitions(VinlandVikings_Lib
		PUBLIC
			NOMINMAX
			_CRT_SECURE_NO_WARNINGS
	)

	#  /permissive-
	target_compile_options(VinlandVikings_Lib
		PUBLIC
			/W3 /WX
			/MP
	)
else ()
	target_compile_options(VinlandVikings_Lib
		PRIVATE
			-Wall -pedantic -Wcast-align -Woverloaded-virtual -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function
			 -fconcepts-diagnostics-depth=2
	)
endif ()

target_link_libraries(VinlandVikings_Lib
	PUBLIC
		Threads::Threads
		CppSdl2::CppSdl2
		nlohmann_json
		nlohmann_json::nlohmann_json
		fmt::fmt
		bfg::lyra
)

set_target_properties(VinlandVikings_Lib
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)
