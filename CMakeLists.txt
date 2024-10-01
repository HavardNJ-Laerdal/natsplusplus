cmake_minimum_required(VERSION 3.22)

# Set the project name
project(natsplusplus VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

# Find the QtCore library
find_package(Qt5 COMPONENTS Core REQUIRED)

# Include the Qt5 directories
include_directories(${Qt5Core_INCLUDE_DIRS})

# Add qt-nats as a subdirectory
add_subdirectory(./qtnats)
include_directories(./qtnats/src)
include_directories(${cnats_SOURCE_DIR} ${cnats_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/qtnats)
message(STATUS "The value of cnats_SOURCE_DIR is: ${cnats_SOURCE_DIR}")

# Add an executable
add_executable(natsplusplus main.cpp NatsMessageBusThingy.cpp NatsMessageBusThingy.h MessageBusInterface.h)

# Link the QtCore library to the executable
target_link_libraries(natsplusplus Qt5::Core ws2_32 qtnats)

