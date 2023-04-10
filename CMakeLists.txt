cmake_minimum_required(VERSION 3.23)
project(RailNetwork)

set(CMAKE_CXX_STANDARD 17)

add_executable(RailNetwork src/main.cpp src/App.cpp src/App.h src/RailManager.cpp src/RailManager.h src/CSVReader.cpp src/CSVReader.h src/RailNetwork.cpp src/RailNetwork.h src/Station.h src/Segment.h)