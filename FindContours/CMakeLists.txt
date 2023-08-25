cmake_minimum_required (VERSION 3.8)

add_executable (Project "main.cpp")

find_package(OpenCV REQUIRED)

target_link_libraries(Project ${OpenCV_LIBS})
