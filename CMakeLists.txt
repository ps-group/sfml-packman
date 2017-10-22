project(sfml-packman)
cmake_minimum_required(VERSION 2.8)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

function(add_sample NAME)
    file(GLOB SAMPLE_SRC "${NAME}/*.cpp" "${NAME}/*.h")
    add_executable(${NAME} ${SAMPLE_SRC})
    target_include_directories(${NAME} PRIVATE ${SFML_INCLUDE_DIR})
    target_compile_features(${NAME} PUBLIC cxx_std_17)
    target_compile_definitions(${NAME} PRIVATE SFML_STATIC)
    target_link_libraries(${NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endfunction()

add_sample(pacman_1)
add_sample(pacman_2)
add_sample(pacman_3)
add_sample(pacman_4)
add_sample(pacman_5)
add_sample(pacman_6)
add_sample(pacman_7)
add_sample(pacman_8)

# Выполняем копирование файлов в выходную директорию
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
