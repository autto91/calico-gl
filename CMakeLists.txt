# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.14)

project("calico-engine")
set(CMAKE_CXX_STANDARD 17)

# Setup Compile Commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


# Include sub-projects.
add_subdirectory(calico)
add_subdirectory(sandbox)
