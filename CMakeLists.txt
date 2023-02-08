cmake_minimum_required(VERSION 2.8.3)
project(manual_matching)

if( ${CMAKE_VERSION} VERSION_LESS "3.8.2" )
    set(CMAKE_CXX_STANDARD 14)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE " -O3 -march=native")

##################
## Dependencies ##
##################

find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
  message(STATUS "Found OpenCV lib : ${OpenCV_VERSION} - ${OpenCV_DIR}")
endif (OpenCV_FOUND)


###########
## Build ##
###########

add_executable(${PROJECT_NAME} source/main.cpp)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  ${OpenCV_LIBS}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)