include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
	GIT_REPOSITORY
		https://github.com/mwthinker/CppSdl2.git
	GIT_TAG
		db81df3d3910ce4400b49ade6d7220de41b3a0f5
)
FetchContent_MakeAvailable(CppSdl2)

Fetchcontent_Declare(CmakeAuxiliary
	GIT_REPOSITORY
		https://gist.github.com/mwthinker/e0539b200e3f0ac6da545af4843a6ff6
)
FetchContent_MakeAvailable(CmakeAuxiliary)
FetchContent_GetProperties(CmakeAuxiliary
	SOURCE_DIR
		CmakeAuxiliary_SOURCE_DIR
)
include(${CmakeAuxiliary_SOURCE_DIR}/auxiliary.cmake)

# Load data.
FetchContent_Declare(VinlandVikingsData
	GIT_REPOSITORY
		#git@github.com:mwthinker/VinlandVikingsData.git
		https://github.com/mwthinker/VinlandVikingsData.git
	GIT_TAG
		#origin/VinlandVikingsData
		34934d26bc503d6a471e329860f9902af56202d1
)
FetchContent_MakeAvailable(VinlandVikingsData)
FetchContent_GetProperties(VinlandVikingsData
	SOURCE_DIR
		VinlandVikingsData_SOURCE_DIR
)
