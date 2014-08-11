cmake_minimum_required(VERSION 2.8)
project(eos_client)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR}/../../)

file(GLOB SRC_ROOT ${PROJECT_DIR}/platform/linux/*.h ${PROJECT_DIR}/platform/linux/*.cpp)
file(GLOB SRC_GAME ${PROJECT_DIR}/game/*.h ${PROJECT_DIR}/game/*.cpp)
file(GLOB SRC_GAME_GRAPHICS ${PROJECT_DIR}/game/graphics/*.h ${PROJECT_DIR}/game/graphics/*.cpp)
file(GLOB SRC_GAME_UTILS ${PROJECT_DIR}/game/utils/*.h ${PROJECT_DIR}/game/utils/*.cpp)

set(SOURCE_FILES
	${SRC_ROOT}
	${SRC_GAME}
	${SRC_GAME_GRAPHICS}
	${SRC_GAME_UTILS}
)

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
	set(OGL_LIB ${OPENGL_LIBRARIES})
else (OPENGL_FOUND)
	message("OpenGL environment missing")
endif (OPENGL_FOUND)

# google-glog
set(GLOG_LIB_DIR ${CMAKE_BINARY_DIR}/libs/glog/lib)
message(STATUS "Searching for glog library: ${GLOG_LIB_DIR}")
set(GLOG_INCLUDE_DIR ${CMAKE_BINARY_DIR}/libs/glog/include)
find_library(GLOG_LIBRARY
	NAMES glog
	PATHS ${GLOG_LIB_DIR}
)
if(NOT GLOG_LIBRARY)
	message(STATUS "Searching for glog library: ${GLOG_LIB_DIR} -- not found")
	# google-glog compilation and linking libglog.so
	message(STATUS "Building glog in ${CMAKE_BINARY_DIR}/libs/glog-compile...")
	execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/libs/glog-compile)
	execute_process(
		COMMAND ${PROJECT_DIR}/lib/glog/configure --prefix=${CMAKE_BINARY_DIR}/libs/glog
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libs/glog-compile
		OUTPUT_QUIET
		OUTPUT_FILE "configure.log"
	)
	message(STATUS "Installing glog in ${CMAKE_BINARY_DIR}/libs/glog...")
	execute_process(
		COMMAND make install -j4
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libs/glog-compile
		OUTPUT_QUIET
	)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(GLOG_LIBRARY ${CMAKE_BINARY_DIR}/libs/glog/lib64/libglog.so)
	else(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(GLOG_LIBRARY ${CMAKE_BINARY_DIR}/libs/glog/lib/libglog.so)
	endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif(NOT GLOG_LIBRARY)
message(STATUS "GLog includes: ${GLOG_INCLUDE_DIR} Libraries: ${GLOG_LIBRARY}" )


include_directories(
	${PROJECT_DIR}/game
	${PROJECT_DIR}/game/graphics
	${PROJECT_DIR}/game/utils
	${GLOG_INCLUDE_DIR}
)

set(EXT_LIBRARIES
	${OGL_LIB}
	${GLEW_LIB}
	${SDL_LIB}
	${SDLMIXER_LIB}
	${GLOG_LIBRARY}
)

include(${PROJECT_DIR}/cmake_build/common/game.cmake)


list(APPEND GAME_DEFINES
	__DESKTOP__
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	COMPILE_DEFINITIONS "${GAME_DEFINES}"
)
