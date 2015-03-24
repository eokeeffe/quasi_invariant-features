cmake_minimum_required (VERSION 2.8)
project (ColourFeatures)

find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})
add_executable(main main.cc gder.cc mboost.cc spherical_der.cc opponent_der.cc HSI_der.cc)
target_link_libraries(main ${OpenCV_LIBS})
