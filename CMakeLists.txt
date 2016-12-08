project(sfml-packman)
cmake_minimum_required(VERSION 2.8)

if(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(DEBUG_LIBS sfml-system sfml-audio sfml-graphics sfml-window)
set(RELEASE_LIBS sfml-system sfml-audio sfml-graphics sfml-window)
else(UNIX)
add_definitions(-DSFML_DYNAMIC)
set(DEBUG_LIBS sfml-system-d sfml-audio-d sfml-graphics-d sfml-window-d opengl32 freetype jpeg winmm)
set(RELEASE_LIBS sfml-system sfml-audio sfml-graphics sfml-window opengl32 freetype jpeg winmm)
endif(UNIX)

macro(add_lesson NUM)
    file(GLOB SRC_LESSON_${NUM} "packman_${NUM}/*.cpp" "packman_${NUM}/*.h")
    add_executable(packman_${NUM} ${SRC_LESSON_${NUM}})
    target_link_libraries(packman_${NUM} debug ${DEBUG_LIBS})
    target_link_libraries(packman_${NUM} optimized ${RELEASE_LIBS})
endmacro(add_lesson)

# Chapter 1
add_lesson(1)
add_lesson(2)
add_lesson(3)
add_lesson(4)
add_lesson(5)
add_lesson(6)

# Chapter 2
add_lesson(7)

# Выполняем копирование файлов в выходную директорию
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
