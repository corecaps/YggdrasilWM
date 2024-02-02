cmake_minimum_required(VERSION 3.12)
project(yggdrasilwm)
set(PROGRAM_NAME "YggdrasilWM")
set(PROGRAM_VERSION "0.1.1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# Include FetchContent module to manage external dependencies
include(FetchContent)

# Download and configure cxxopts
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v2.2.1  # Adjust the version tag as needed
)

FetchContent_GetProperties(cxxopts)
if (NOT cxxopts_POPULATED)
    FetchContent_Populate(cxxopts)
    add_subdirectory(${cxxopts_SOURCE_DIR} ${cxxopts_BINARY_DIR})
endif ()

# Download and configure GoogleTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0  # Adjust the version tag as neededhttps://github.com/google/googletest/releases/tag/v1.14.0
)

FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()

# Find required packages
find_package(X11 REQUIRED)

# Set source and include directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)

# Add include directories for X11, cxxopts, GoogleTest, and GoogleMock
include_directories(${X11_INCLUDE_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${cxxopts_SOURCE_DIR})
include_directories(${googletest_SOURCE_DIR}/googletest/include)
include_directories(${googletest_SOURCE_DIR}/googlemock/include)

# Add your source files
set(SOURCES
        ${SOURCE_DIR}/window_manager.cpp

        ${SOURCE_DIR}/Logger.cpp
        ${SOURCE_DIR}/Client.cpp
        ${INCLUDE_DIR}/Client.hpp
        ${SOURCE_DIR}/EventHandler.cpp
        ${INCLUDE_DIR}/EventHandler.hpp
        ${SOURCE_DIR}/LayoutManager.cpp
        ${INCLUDE_DIR}/LayoutManager.hpp
        ${SOURCE_DIR}/ConfigHandler.cpp
        ${INCLUDE_DIR}/ConfigHandler.hpp
        src/TreeLayoutManager.cpp
        inc/TreeLayoutManager.hpp
)

# Create the executable
add_executable(${PROGRAM_NAME} ${SOURCES} ${SOURCE_DIR}/main.cpp)

# Link against X11, cxxopts, GoogleTest, and GoogleMock libraries
target_link_libraries(${PROGRAM_NAME}
        ${X11_LIBRARIES}
        cxxopts
        gtest
        gmock
)

# Add compile definitions
add_definitions(
        -DPROGRAM_NAME="${PROGRAM_NAME}"
        -DPROGRAM_VERSION="${PROGRAM_VERSION}"
)
# Set C++ standard
set_property(TARGET ${PROGRAM_NAME} PROPERTY CXX_STANDARD 14)

# Enable testing
enable_testing()

# Add the test files
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_SOURCES ${TEST_DIR}/*.cpp)

# Create the test executable
add_executable(${PROGRAM_NAME}_tests ${SOURCES} ${TEST_SOURCES} )

# Link against X11, cxxopts, GoogleTest, and GoogleMock libraries
target_link_libraries(${PROGRAM_NAME}_tests
        ${X11_LIBRARIES}
        cxxopts
        gtest
        gmock
        gtest_main
)

# Set C++ standard for tests
set_property(TARGET ${PROGRAM_NAME}_tests PROPERTY CXX_STANDARD 14)

# Add the test as a target for running with 'make test'
add_test(NAME ${PROGRAM_NAME}_tests COMMAND ${PROGRAM_NAME}_tests)