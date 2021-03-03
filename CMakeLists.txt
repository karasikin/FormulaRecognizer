cmake_minimum_required(VERSION 3.19) 
project(formula_recognizer)

find_package(ImageMagick COMPONENTS Magick++ REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(SRC_LIST 
    src/main.cpp
    src/symbol_slicer/slicer.cpp)

include_directories(${ImageMagick_Magick++_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
