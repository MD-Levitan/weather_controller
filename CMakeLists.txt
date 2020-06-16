cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_FLAGS  "-Werror=format-truncation=1")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(weather_controller)

