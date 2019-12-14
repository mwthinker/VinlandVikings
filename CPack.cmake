
install(TARGETS VinlandVikings
	RUNTIME
		DESTINATION .
	COMPONENT application
)

install(DIRECTORY ${VinlandVikingsData_SOURCE_DIR}/fonts/
	DESTINATION .
	COMPONENT data
)

install(DIRECTORY ${VinlandVikingsData_SOURCE_DIR}/images/
	DESTINATION .
	COMPONENT data
)

# Replace the USE_APPLICATION_JSON file, use default saving place.
#install(FILES ${CMAKE_SOURCE_DIR}/data/package/USE_APPLICATION_JSON
#	DESTINATION .
#	COMPONENT data
#)

if (MSVC AND DLL_FOLDER)
	########################################################################
	# Tell CMake to install the windows runtime libraries to the programs
	# directory and tell CPack that they belong to the applications component
	include(InstallRequiredSystemLibraries)
	install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
		DESTINATION .
		COMPONENT application
	)
endif ()

set(CPACK_PACKAGE_CONTACT "mwthinker@yahoo.com")

# CPack configuration
set(CPACK_PACKAGE_NAME "VinlandVikings")

set(CPACK_PACKAGE_VENDOR "MW")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Installs VinlandVikings")
set(CPACK_PACKAGE_VERSION_MAJOR ${VinlandVikings_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VinlandVikings_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VinlandVikings_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VinlandVikings")	

set(CPACK_GENERATOR ZIP)

# Tell CPack about the components and group the data components together.
if (MSVC)
	set(CPACK_COMPONENTS_ALL application data runtime vcredist)
else ()
	set(CPACK_COMPONENTS_ALL application data)
endif ()

# More descriptive names for each of the components, and component groups.
set(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "Application")
set(CPACK_COMPONENT_APPLICATION_REQUIRED true)

set(CPACK_COMPONENT_DATA_DISPLAY_NAME "Data")
set(CPACK_COMPONENT_DATA_REQUIRED true)

set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime")
set(CPACK_COMPONENT_RUNTIME_REQUIRED true)

# Text from "LICENSE.txt" is displayed in the installer's license tab.
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
#set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\data\\\\package\\\\mw.bmp")

set(CPACK_PACKAGE_EXECUTABLES "VinlandVikings" "VinlandVikings")
set(CPACK_CREATE_DESKTOP_LINKS "VinlandVikings")

include(CPack)