cmake_minimum_required(VERSION 3.0)
project(dlock13-msgflo CXX)

# Use C++11
add_compile_options(-std=c++11)

# MsgFlo
find_package(MsgFlo REQUIRED)
find_library(libev_LIB ev)

# dlock13
add_executable(dlock13 dlock13.cpp)
target_include_directories(dlock13 PUBLIC MsgFlo::msgflo)
target_link_libraries(dlock13 MsgFlo::msgflo pthread)
