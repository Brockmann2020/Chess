cmake_minimum_required(VERSION 3.0)
project(Chess CXX)

set(EXECUTABLE_NAME Chess)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

set(SRC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/gui/boardmanager.cpp
        src/engine/board.cpp
)

set(INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/include/gui
        ${CMAKE_CURRENT_LIST_DIR}/include/engine
)

add_subdirectory(libs/raylib)

add_executable(${EXECUTABLE_NAME} ${SRC_FILES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INCLUDE_DIRS})

target_link_libraries(Chess PRIVATE raylib)
target_compile_definitions(Chess PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/svg/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(Chess PUBLIC ASSETS_PATH="relative-path-to-assets-in-the-game-package") # Set the asset path macro in release more
