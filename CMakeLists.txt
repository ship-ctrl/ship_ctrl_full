# Specify the minimum version.
cmake_minimum_required(VERSION 3.9)

# Specify the project info.
project(ship_ctrl_full VERSION 0.1.0 DESCRIPTION "Ship communication protocol over udp non_library version")

# Set the C++ standard to be used
set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto")

enable_testing()

find_package(Boost REQUIRED)

find_package(glog REQUIRED)

# Include headers
include_directories(include)

add_subdirectory( src build/src )
add_subdirectory( test build/test )




# Add precompiled header file
#set(PRECOMPILED_HEADER ${CMAKE_SOURCE_DIR}/include/pch.h)
#set_source_files_properties(${PRECOMPILED_HEADER} PROPERTIES COMPILE_FLAGS "/Yc\"pch.h\" /Fppch.h")
#list(APPEND SOURCE_FILES ${PRECOMPILED_HEADER})

# Add source files
#file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Declare the library target.
#add_library(${PROJECT_NAME} SHARED
#    src/DllShip.cpp
#    include/DllShip.h
#)



# Optional: Specify library directories
#link_directories(lib)

# Optional: Link against external libraries
#target_link_libraries(MyProject library1 library2)

# Configure the directories to search for header files.
#target_include_directories(${PROJECT_NAME} PRIVATE src)

# Set the version property.
#set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Set the shared object version property to the project's major version.
#set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

# Set the public header property to the one with the actual API.
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/DllShip.h)