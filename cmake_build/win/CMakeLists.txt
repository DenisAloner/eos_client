cmake_minimum_required(VERSION 2.8)
project(eos_client)

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../)

file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.cpp)

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib/glog/src/windows
)

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
	set(OGL_LIB ${OPENGL_LIBRARIES})
else (OPENGL_FOUND)
	message("OpenGL environment missing")
endif (OPENGL_FOUND)

find_library(GLEW_LIB glew32
	PATHS ${PROJECT_SOURCE_DIR}/lib/win/${LIB_PLATFORM}
	DOC "OpenGL libraries"
)

find_library(SDL_LIB SDL2
	PATHS ${PROJECT_SOURCE_DIR}/lib/win/${LIB_PLATFORM}
	DOC "SDL libraries"
)

find_library(SDLMIXER_LIB SDL2_Mixer
	PATHS ${PROJECT_SOURCE_DIR}/lib/win/${LIB_PLATFORM}
	DOC "SDL libraries"
)

find_library(GLOG_LIB libglog
	PATHS ${PROJECT_SOURCE_DIR}/lib/win/${LIB_PLATFORM}
	DOC "Logging library"
)

set(EXT_LIBRARIES
	${OGL_LIB}
	${GLEW_LIB}
	${SDL_LIB}
	${SDLMIXER_LIB}
	${GLOG_LIB}
)

include(${PROJECT_SOURCE_DIR}/cmake_build/common/game.cmake)

add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	COMPILE_DEFINITIONS "${GAME_DEFINES}"
)
