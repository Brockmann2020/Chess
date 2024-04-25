cmake_minimum_required(VERSION 3.26)
project(ChessBot)

set(CMAKE_CXX_STANDARD 17)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

set(EXECUTABLE_NAME ChessBot)

set(SRC_FILES
        src/main.cpp
        src/gui/chessboard.cpp
        src/engine/board.cpp
        src/engine/board.cpp
        src/engine/piece.cpp
        src/gui/chesspiece.cpp
)

set(INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/include/gui
        ${CMAKE_CURRENT_LIST_DIR}/include/engine
        ${CMAKE_CURRENT_LIST_DIR}/lib/raylib/include
)

add_library(raylib lib/raylib/lib/raylib.dll)

set_target_properties(raylib PROPERTIES LINKER_LANGUAGE C)

#add_subdirectory(lib/raylib/lib)

add_executable(ChessBot ${SRC_FILES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INCLUDE_DIRS})

target_link_libraries(ChessBot PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/raylib/lib/raylib.dll)
target_compile_definitions(ChessBot PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
