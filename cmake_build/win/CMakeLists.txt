cmake_minimum_required(VERSION 2.8)
project(eos_client)

set (PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../)

file(GLOB source_files ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.cpp)

# source_group("Actions" FILES
# 	TAction.h, TAction.cpp,
# 	ActionsDescription.h,
# 	Action_OpenInventory.h, Action_OpenInventory.cpp
# )

include_directories(
	${PROJECT_SOURCE_DIR}/include
)

find_library(OGL_LIB opengl32
	PATHS ${PROJECT_SOURCE_DIR}/lib
	DOC "OpenGL libraries"
)
find_library(GLU_LIB glu32
	PATHS ${PROJECT_SOURCE_DIR}/lib
	DOC "OpenGL libraries"
)
find_library(GLEW_LIB glew32
	PATHS ${PROJECT_SOURCE_DIR}/lib
	DOC "OpenGL libraries"
)
find_library(SDL_LIB SDL2
	PATHS ${PROJECT_SOURCE_DIR}/lib
	DOC "SDL libraries"
)
find_library(SDLMIXER_LIB SDL2_Mixer
	PATHS ${PROJECT_SOURCE_DIR}/lib
	DOC "SDL libraries"
)
set(EXT_LIBRARIES
	${OGL_LIB}
	${GLU_LIB}
	${GLEW_LIB}
	${SDL_LIB}
	${SDLMIXER_LIB}
)

add_executable(${PROJECT_NAME} WIN32 ${source_files})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
