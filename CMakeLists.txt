# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(ToyRedis)

set(CMAKE_OSX_ARCHITECTURES arm64)


set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
endif()


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
