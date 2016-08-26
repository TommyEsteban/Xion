cmake_minimum_required (VERSION 3.0.2)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_policy(SET CMP0048 NEW)
project(xion VERSION 1.1.0)
set(CMAKE_C_FLAGS "-O -Wall -std=gnu99 -g")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(EXECUTABLE_TEST_DIR ${PROJECT_SOURCE_DIR}/bin/xionTest)

configure_file ("${PROJECT_SOURCE_DIR}/include/xionConfig.h.in"
"${PROJECT_SOURCE_DIR}/include/xionConfig.h"
  )

find_package(JPEG REQUIRED)
find_package(CUDA REQUIRED)

file(GLOB_RECURSE XION_SOURCES ${PROJECT_SOURCE_DIR}/modules/*.c)
file(GLOB_RECURSE TESTING_SOURCES ${PROJECT_SOURCE_DIR}/test/*.c)
file(GLOB_RECURSE CUDA_SOURCES ${PROJECT_SOURCE_DIR}/modules/cuda/*.cu)
list(APPEND CUDA_NVCC_FLAGS "-g -gencode arch=compute_35,code=sm_35;")

link_directories(${JPEG_LIBRARIES})
include_directories(${JPEG_INCLUDE_DIR})

cuda_add_executable(xion ${XION_SOURCES} ${CUDA_SOURCES})
cuda_add_library(xionTesting SHARED ${XION_SOURCES} ${CUDA_SOURCES})

target_link_libraries(xion ${JPEG_LIBRARIES})

link_directories(${LIBRARY_OUTPUT_PATH})
include_directories(${INCLUDE_DIR})
cuda_add_executable(xionTest ${TESTING_SOURCES})
target_link_libraries(xionTest xionTesting)
target_link_libraries(xionTest ${JPEG_LIBRARIES})

enable_testing()
set(ENV{CTEST_OUTPUT_ON_FAILURE} TRUE)
add_test(NAME dataset COMMAND ${EXECUTABLE_TEST_DIR} "dataset")
add_test(NAME preprocessing COMMAND ${EXECUTABLE_TEST_DIR} "preprocessing")