cmake_minimum_required(VERSION 3.19) 
project(dataset_builder)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall")

set(SRC_LIST 
    src/main.cpp
    src/datasetbuilder.cpp
    src/imageviewer.cpp
    ../src/dataset/dataset.cpp
    ../src/segmenter/formula.cpp
    ../src/segmenter/rect.cpp
)

include_directories(../include)

add_executable(${PROJECT_NAME} ${SRC_LIST})
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES} Qt5::Widgets)
