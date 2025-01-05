project(VinlandVikings_Test
	DESCRIPTION
		"Testing project using GTest"
	LANGUAGES
		CXX
)

find_package(GTest CONFIG REQUIRED)
enable_testing()

set(SOURCES_TEST
	src/vinlandvikings.cpp
	src/main.cpp

	CMakeLists.txt
)

set_property(GLOBAL PROPERTY USE_FOLDERS On)
source_group(TREE
	${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES_TEST}
)

add_executable(VinlandVikings_Test
	${SOURCES_TEST}
)

target_link_libraries(VinlandVikings_Test
	PUBLIC
		GTest::gtest GTest::gmock
		VinlandVikings::VinlandVikings_Lib
)

if (MSVC)
	target_compile_options(VinlandVikings_Test
		PRIVATE
			"/permissive-"
	)
endif ()

set_target_properties(VinlandVikings_Test
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

include(GoogleTest)
gtest_discover_tests(VinlandVikings_Test)
