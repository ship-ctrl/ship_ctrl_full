# Specify the minimum version.
cmake_minimum_required(VERSION 3.9)

# Specify the project info.
project(ship_ctrl_full VERSION 0.1.0 DESCRIPTION "Ship communication protocol over udp non_library version")

if(CMAKE_VERSION VERSION_LESS "3.29.0") 
    message("Please consider to switch to CMake 3.29.0 or stay in hell with no lib dedup in case of building cv with intrinsics")

else()
    cmake_policy(SET CMP0156 NEW) #deduplicate lib
    cmake_policy(SET CMP0179 NEW) #use first occurrence
endif()

# Include the module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(AutoVersion)
include(FetchContent)

# Set the C++ standard to be used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O3 -flto")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto")

set(VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/version_info.txt")

# Increment build number each time CMake runs
increment_and_save_build(${CMAKE_CURRENT_SOURCE_DIR})

# Read back the updated values for use in CMake
read_version_from_file(${CMAKE_CURRENT_SOURCE_DIR})

option(INCLUDE_GLOG "fetch glog and link to main target" OFF)
option(INCLUDE_GFLAGS "fetch gflags and link to main target" OFF)

option(BUILD_TEST_lo "Build the testing tree" ON)
option(INCLUDE_BOOST "fetch boost and link to main target" OFF)

configure_file( 
    ${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/Version.h
    )

if(BUILD_TEST_lo EQUAL "ON")
    if(INCLUDE_BOOST OR RUNNER_FETCH_ALL)
        FetchContent_Declare(
            Boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            #GIT_TAG boost-1.85.0
        )
        FetchContent_MakeAvailable(Boost)
    else()
        message (STATUS "Boost found or RUNNER_FETCH_ALL = ${RUNNER_FETCH_ALL}")
        find_package (Boost COMPONENTS unit_test_framework filesystem REQUIRED CONFIG)
    endif(INCLUDE_BOOST OR RUNNER_FETCH_ALL)
    
    enable_testing()
    include(CTest)
    message("testdir is ${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
        add_subdirectory(test)
    endif()
    if(Boost_FOUND)
        message ("Boost found")
    else()
        #message (FATAL_ERROR "Cannot find Boost")
    endif(Boost_FOUND)
endif(BUILD_TEST_lo EQUAL "ON")

option(FORCE_FETCH_DEPS "fetching deps via fetchcontent" ON)
# option(INCLUDE_GLOG "fetch glog and link to main target" OFF)
# option(INCLUDE_GFLAGS "fetch gflags and link to main target" OFF)

if(FORCE_FETCH_DEPS EQUAL "ON")
    # FetchContent for both gflags and glog
    FetchContent_Declare(
        gflags
        GIT_REPOSITORY https://github.com/gflags/gflags.git
        GIT_TAG main
    )
    FetchContent_Declare(
        glog
        GIT_REPOSITORY https://github.com/google/glog.git
        # GIT_TAG v0.6.0
    )
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
    set(WITH_GFLAGS OFF CACHE BOOL "Build with gflags")  # Or ON if glog needs it
    set(WITH_UNWIND OFF CACHE BOOL "Build with unwind support")
    FetchContent_MakeAvailable(gflags glog)
else()
    message(STATUS "FFD: ${FORCE_FETCH_DEPS}")
    find_package(gflags REQUIRED)
    find_package(glog REQUIRED)
endif(FORCE_FETCH_DEPS EQUAL "ON")

# enable_testing()

# find_package(Boost REQUIRED)

# find_package(glog REQUIRED)

message(STATUS "==========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${FULL_VERSION}")
message(STATUS "Build Number: ${BUILD_NUMBER}")
message(STATUS "Minor Version: ${MINOR_VERSION}")
message(STATUS "==========================================")



# Include headers
include_directories(
    include
    # ${Boost_INCLUDE_DIR}
    )

# add_subdirectory( src build/src )
# add_subdirectory( test build/test )




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