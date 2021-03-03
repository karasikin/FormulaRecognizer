cmake_minimum_required(VERSION 3.19) 
project(formula_recognizer)

# find_package(ImageMagick COMPONENTS Magick++ MagickCore R)
# find_program(MAGICK_CONFIG "Magick++-config")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(SRC_LIST 
    src/main.cpp
    src/symbol_slicer/slicer.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})

# include_directories(${ImageMagick_Magick++_INCLUDE_DIRS})
# add_executable(${PROJECT_NAME} ${SRC_LIST})
# target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
