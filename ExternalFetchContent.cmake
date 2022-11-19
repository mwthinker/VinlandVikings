include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
	GIT_REPOSITORY
		https://github.com/mwthinker/CppSdl2.git
	GIT_TAG
		e08e3c2a990b196af190c2d7df6ebcfa1642708d
	OVERRIDE_FIND_PACKAGE
)

Fetchcontent_Declare(CmakeAuxiliary
	GIT_SHALLOW 1
	GIT_REPOSITORY
		https://gist.github.com/mwthinker/e0539b200e3f0ac6da545af4843a6ff6
)
FetchContent_MakeAvailable(CmakeAuxiliary)
FetchContent_GetProperties(CmakeAuxiliary
	SOURCE_DIR
		CmakeAuxiliary_SOURCE_DIR
)
include(${CmakeAuxiliary_SOURCE_DIR}/auxiliary.cmake)

message(STATUS "Download private data repository is available to add: -DZombieData_PrivateRepo_SSH=1")
message(STATUS "Download private data repository is available to add: -DZombieData_PrivateRepo_HTTPS=1")
option(VinlandVikingsData_PrivateRepo_SSH "Add VinlandVikingsData_PrivateRepo_SSH to project." OFF)
option(VinlandVikingsData_PrivateRepo_HTTPS "Add VinlandVikingsData_PrivateRepo_HTTPS to project." OFF)


if (VinlandVikingsData_PrivateRepo_SSH OR VinlandVikingsData_PrivateRepo_HTTPS)
	set(VinlandVikingsData ON)
	set(VinlandVikingsData_PrivateRepo_URL "https://github.com/mwthinker/VinlandVikingsData.git")
	if (VinlandVikingsData_PrivateRepo_SSH)
		set(VinlandVikingsData_PrivateRepo_URL "git@github.com:mwthinker/VinlandVikingsData.git")
	endif ()
	# Load data.
	FetchContent_Declare(VinlandVikingsData
		GIT_REPOSITORY
			${VinlandVikingsData_PrivateRepo_URL}
		GIT_TAG
			#origin/VinlandVikingsData
			34934d26bc503d6a471e329860f9902af56202d1
	)
	FetchContent_MakeAvailable(VinlandVikingsData)
	FetchContent_GetProperties(VinlandVikingsData
		SOURCE_DIR
			VinlandVikingsData_SOURCE_DIR
	)
endif ()
