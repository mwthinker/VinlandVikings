cmake_minimum_required(VERSION 3.0)
project(Config)

find_package(Protobuf REQUIRED)

set(PROTO_FILES
	"config.proto"
)

# Hide options in cmake
mark_as_advanced(FORCE Protobuf_SRC_ROOT_FOLDER)
mark_as_advanced(FORCE Protobuf_DIR)

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
add_library(Config ${PROTO_HEADER} ${PROTO_SRC} ${PROTO_FILES})

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4005") # 'identifier' : macro redefinition
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") # 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") # An integer type is converted to a smaller integer type
endif ()

target_include_directories(Config
	PUBLIC
		${PROTOBUF_INCLUDE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(Config
	PUBLIC
		protobuf::libprotobuf
	PRIVATE
		protobuf::libprotoc
)
