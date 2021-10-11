cmake_minimum_required(VERSION 3.19)
project(zadanie_4 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
link_libraries(m)

include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/rng.c)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/data.c)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/main.c)

add_executable(zadanie_4 ${SRC_FILES})