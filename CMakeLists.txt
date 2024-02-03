cmake_minimum_required(VERSION 3.16...3.23)
project(ray_tracing)

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
