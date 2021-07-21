cmake_minimum_required(VERSION 3.19)
project(Thre_seg)

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR "/home/wya/Project/3rdparty/install/opencv-3.4.10/share/OpenCV")

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(Thre_seg main.cpp)
target_link_libraries(Thre_seg  ${OpenCV_LIBS})