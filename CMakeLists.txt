project(whc)

cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_FLAGS "-Wall -O2")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(LibSSH REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

set(QT_USE_QTXML TRUE)

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(include)
