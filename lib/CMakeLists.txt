cmake_minimum_required(VERSION 3.0.0)
project(MySql VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(../include)
include_directories(/usr/include)
add_library(MySql SHARED MySql.cc)

