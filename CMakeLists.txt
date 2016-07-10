project(sfml-packman)
cmake_minimum_required(VERSION 2.8)

if(UNIX)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(PROJECT_LIBS sfml-system sfml-audio sfml-graphics sfml-window)
endif(UNIX)

aux_source_directory(packman_kerim_2 SRC_LIST_KERIM_2)
add_executable(packman_kerim_2 ${SRC_LIST_KERIM_2})
target_link_libraries(packman_kerim_2 ${PROJECT_LIBS})

aux_source_directory(packman_kerim_3 SRC_LIST_KERIM_3)
add_executable(packman_kerim_3 ${SRC_LIST_KERIM_3})
target_link_libraries(packman_kerim_3 ${PROJECT_LIBS})

aux_source_directory(packman_1 SRC_LIST_1)
add_executable(packman_1 ${SRC_LIST_1})
target_link_libraries(packman_1 ${PROJECT_LIBS})

aux_source_directory(packman_2 SRC_LIST_2)
add_executable(packman_2 ${SRC_LIST_2})
target_link_libraries(packman_2 ${PROJECT_LIBS})

aux_source_directory(packman_3 SRC_LIST_3)
add_executable(packman_3 ${SRC_LIST_3})
target_link_libraries(packman_3 ${PROJECT_LIBS})

aux_source_directory(packman_4 SRC_LIST_4)
add_executable(packman_4 ${SRC_LIST_4})
target_link_libraries(packman_4 ${PROJECT_LIBS})
