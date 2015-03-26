cmake_minimum_required (VERSION 2.8)
project (ColourFeatures)

find_package(OpenCV REQUIRED)

include_directories(
    ${OPENCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/Include/
)

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O3")

add_executable(main
    src/main.cc
    src/gder.cc
    src/mboost.cc
    src/spherical_der.cc
    src/opponent_der.cc
    src/HSI_der.cc
    src/operations.cc
    src/colourStructureTensor.cc
    src/colourHarris.cc
    src/colourCanny.cc
)
target_link_libraries(main ${OpenCV_LIBS})
