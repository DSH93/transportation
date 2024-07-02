cmake_minimum_required(VERSION 3.25)
project(HW02ADV)

set(CMAKE_CXX_STANDARD 11)

add_executable(HW02ADV netOmsterdam.cpp MultiGraph.cpp MultiGraph.h Edge.cpp Edge.h Station.cpp Station.h interaction.cpp interaction.h)
