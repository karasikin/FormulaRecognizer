cmake_minimum_required(VERSION 3.19) 
project(formula_recognizer)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall")

set(SRC_LIST 
    src/main.cpp
    src/recognizer.cpp
    src/segmenter/formula.cpp
    src/segmenter/rect.cpp
    src/neural_network/matrix.cpp
)

set(HEADER_DIRECTORY include)

include_directories(${HEADER_DIRECTORY})

add_executable(${PROJECT_NAME} ${SRC_LIST})
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})

