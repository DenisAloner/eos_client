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
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/lib
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/freetype2
)

add_executable(${PROJECT_NAME} ${source_files})
