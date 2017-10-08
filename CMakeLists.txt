cmake_minimum_required(VERSION 3.8)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Vec3.cpp Vec3.h Circle.cpp Circle.h)
add_executable(RayTracer ${SOURCE_FILES})