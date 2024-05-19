cmake_minimum_required(VERSION 3.10)
project(QRReader)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(qr_reader main.cpp)
target_link_libraries(qr_reader ${OpenCV_LIBS})

