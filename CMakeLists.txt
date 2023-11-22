cmake_minimum_required(VERSION 3.12)
project(yggdrasilwm)
set(PROGRAM_NAME "YggdrasilWM")
set(PROGRAM_VERSION "0.1.1")
# Include FetchContent module to manage external dependencies
include(FetchContent)
# Download and configure cxxopts
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v2.2.1  # Adjust the version tag as needed
)

FetchContent_GetProperties(cxxopts)
if(NOT cxxopts_POPULATED)
    FetchContent_Populate(cxxopts)
    add_subdirectory(${cxxopts_SOURCE_DIR} ${cxxopts_BINARY_DIR})
endif()
# Find required packages
find_package(X11 REQUIRED)
# Set source and include directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
# Add include directories for X11 and cxxopts
include_directories(${X11_INCLUDE_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${cxxopts_SOURCE_DIR})

# Add your source files
set(SOURCES
  ${SOURCE_DIR}/window_manager.cpp
  ${SOURCE_DIR}/util.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/Logger.cpp
)

# Create the executable
add_executable(${PROGRAM_NAME} ${SOURCES})

# Link against X11 and cxxopts libraries
target_link_libraries(${PROGRAM_NAME}
    ${X11_LIBRARIES}
    cxxopts
)
# Add compile definitions
target_compile_definitions(${PROGRAM_NAME}
        PRIVATE PROGRAM_NAME="${PROGRAM_NAME}"
        PROGRAM_VERSION="${PROGRAM_VERSION}"
)
# Set C++ standard
set_property(TARGET ${PROGRAM_NAME} PROPERTY CXX_STANDARD 14)
