install(TARGETS ${CMAKE_PROJECT_NAME}
	RUNTIME DESTINATION .
	COMPONENT application
)

install(DIRECTORY ${VinlandVikingsData_SOURCE_DIR}/fonts/
	DESTINATION fonts
	COMPONENT data
)

install(DIRECTORY ${VinlandVikingsData_SOURCE_DIR}/images/
	DESTINATION images
	COMPONENT data
)

install(DIRECTORY ${VinlandVikingsData_SOURCE_DIR}/installer/
	DESTINATION .
	COMPONENT data
)

# Replace the USE_APPLICATION_JSON file, use default saving place.
#install(FILES ${CMAKE_SOURCE_DIR}/data/package/USE_APPLICATION_JSON
#	DESTINATION .
#	COMPONENT data
#)

if (MSVC)
	# Tell CMake to install the windows runtime libraries to the programs
	# directory and tell CPack that they belong to the "applications" component
	set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
	include(InstallRequiredSystemLibraries)
	file(GLOB DLL_FILES_DEBUG "${CMAKE_BINARY_DIR}/Debug/*.dll")
	file(GLOB DLL_FILES_MINSIZEREL "${CMAKE_BINARY_DIR}/MinSizeRel/*.dll")
	file(GLOB DLL_FILES_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/RelWithDebInfo/*.dll")
	file(GLOB DLL_FILES_RELEASE "${CMAKE_BINARY_DIR}/Release/*.dll")

	set(DLL_FILES "$<$<CONFIG:Debug>:${DLL_FILES_DEBUG}>")
	set(DLL_FILES "${DLL_FILES}$<$<CONFIG:MinSizeRel>:${DLL_FILES_MINSIZEREL}>")
	set(DLL_FILES "${DLL_FILES}$<$<CONFIG:RelWithDebInfo>:${DLL_FILES_RELWITHDEBINFO}>")
	set(DLL_FILES "${DLL_FILES}$<$<CONFIG:Release>:${DLL_FILES_RELEASE}>")

	install(PROGRAMS "${DLL_FILES}"
		DESTINATION .
		COMPONENT application
	)
endif ()

set(CPACK_PACKAGE_CONTACT "mwthinker@yahoo.com")
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")

set(CPACK_PACKAGE_VENDOR "MW")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Installs ${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")

set(CPACK_GENERATOR ZIP)

# Tell CPack about the components and group the data components together.
set(CPACK_COMPONENTS_ALL application data)

# More descriptive names for each of the components, and component groups.
set(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "Application")
set(CPACK_COMPONENT_APPLICATION_REQUIRED true)

set(CPACK_COMPONENT_DATA_DISPLAY_NAME "Data")
set(CPACK_COMPONENT_DATA_REQUIRED true)

set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime")
set(CPACK_COMPONENT_RUNTIME_REQUIRED true)

# Text from "LICENSE.txt" is displayed in the installer's license tab.
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_ICON "${CMAKE_BINARY_DIR}/installer/vinlandvikings.png")

set(CPACK_PACKAGE_EXECUTABLES "${CMAKE_PROJECT_NAME}" "${CMAKE_PROJECT_NAME}")
set(CPACK_CREATE_DESKTOP_LINKS "${CMAKE_PROJECT_NAME}")

# ------------------- WIX ---------------------------------------

if (MSVC)
	option(USE_WIX_GENERATOR "Use WIX generator" OFF)
	if (USE_WIX_GENERATOR)
		set(CPACK_GENERATOR ${CPACK_GENERATOR} WIX)
		set(CPACK_WIX_PRODUCT_GUID "bf1ee6e4-389a-4567-ad2c-61a4d3c71d69")
		set(CPACK_WIX_UPGRADE_GUID "bf1ee6e4-389a-4567-ad2c-61a4d3c71d66")

		set(CPACK_WIX_UI_DIALOG "${CMAKE_BINARY_DIR}/installer/install_dialog.png")
		set(CPACK_WIX_UI_BANNER "${CMAKE_BINARY_DIR}/installer/install_banner.png")
		
		set(CPACK_WIX_PRODUCT_ICON "${CMAKE_BINARY_DIR}/installer/icon.ico")
		
		message(STATUS ${CPACK_PACKAGE_ICON})
		#set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "https://github.com/mwthinker/MWetris")
	endif ()
endif ()

include(CPack)
