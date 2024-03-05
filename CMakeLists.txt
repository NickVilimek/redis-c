# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(ToyRedis)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Source files
set(SOURCES
    # src/server.cpp
    # src/client.cpp
    src/helpers/utils.h
    src/helpers/utils.cpp
)

# Executable targets
add_executable(server "src/server" ${SOURCES})
add_executable(client "src/client" ${SOURCES})
