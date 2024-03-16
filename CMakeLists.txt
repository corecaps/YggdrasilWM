# Yb  dP              8                w 8 Yb        dP 8b   d8
#  YbdP  .d88 .d88 .d88 8d8b .d88 d88b w 8  Yb  db  dP  8YbmdP8
#   YP   8  8 8  8 8  8 8P   8  8 `Yb. 8 8   YbdPYbdP   8  "  8
#   88   `Y88 `Y88 `Y88 8    `Y88 Y88P 8 8    YP  YP    8     8
#        wwdP wwdP
# Yggdrasil Window Manager
# https://github.com/corecaps/YggdrasilWM
# Copyright (C) 2024 jgarcia <jgarcia@student.42.fr> <corecaps@gmail.com>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.

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

# Download and configure jsoncpp
FetchContent_Declare(
        jsoncpp
        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
        GIT_TAG 1.9.4  # Adjust the version tag as needed
)

FetchContent_GetProperties(jsoncpp)
if (NOT jsoncpp_POPULATED)
    FetchContent_Populate(jsoncpp)
    add_subdirectory(${jsoncpp_SOURCE_DIR} ${jsoncpp_BINARY_DIR})
endif()

# Find required packages
include(FindPkgConfig)
find_package(X11 REQUIRED)
pkg_check_modules(XFT REQUIRED xft)


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
        ${SOURCE_DIR}/WindowManager.cpp
        ${SOURCE_DIR}/Logger.cpp
        ${SOURCE_DIR}/Client.cpp
        ${SOURCE_DIR}/EventHandler.cpp
        ${SOURCE_DIR}/Group.cpp
        ${SOURCE_DIR}/Ewmh.cpp
        ${SOURCE_DIR}/Layouts/LayoutManager.cpp
        ${INCLUDE_DIR}/Layouts/LayoutManager.hpp
        ${SOURCE_DIR}/Layouts/TreeLayoutManager.cpp
        ${SOURCE_DIR}/Layouts/BinarySpace.cpp
        ${SOURCE_DIR}/Config/ConfigHandler.cpp
        ${INCLUDE_DIR}/Config/ConfigDataBase.hpp
        ${SOURCE_DIR}/Config/ConfigDataGroups.cpp
        ${SOURCE_DIR}/Config/ConfigDataGroup.cpp
        ${SOURCE_DIR}/Config/ConfigDataBars.cpp
        ${SOURCE_DIR}/Config/ConfigDataBar.cpp
        ${SOURCE_DIR}/Config/ConfigDataWidget.cpp
        ${SOURCE_DIR}/Config/ConfigDataBindings.cpp
        ${SOURCE_DIR}/Config/ConfigFileHandler.cpp
        ${SOURCE_DIR}/Config/Binding.cpp
        ${INCLUDE_DIR}/Config/Binding.hpp
        ${INCLUDE_DIR}/Commands/CommandBase.hpp
        ${SOURCE_DIR}/Commands/FocusGroup.cpp
        ${SOURCE_DIR}/Commands/Spawn.cpp
        ${SOURCE_DIR}/Commands/Quit.cpp
        ${SOURCE_DIR}/Bars/Bars.cpp
        ${SOURCE_DIR}/Bars/Bar.cpp
        ${SOURCE_DIR}/Bars/TSBarsData.cpp
        ${INCLUDE_DIR}/Bars/Widget.hpp
        ${INCLUDE_DIR}/X11wrapper/baseX11Wrapper.hpp
        ${SOURCE_DIR}/X11wrapper/X11Wrapper.cpp
        ${INCLUDE_DIR}/X11wrapper/X11Wrapper.hpp
        ${INCLUDE_DIR}/YggdrasilExceptions.hpp
)

# create the executable
add_executable(${PROGRAM_NAME} ${SOURCES} ${SOURCE_DIR}/main.cpp)

# Link against X11, cxxopts, GoogleTest, and GoogleMock libraries
target_link_libraries(${PROGRAM_NAME}
        ${X11_LIBRARIES}
        cxxopts
        gtest
        gmock
        jsoncpp_lib
)

# Add compile definitions
add_definitions(
        -DPROGRAM_NAME="${PROGRAM_NAME}"
        -DPROGRAM_VERSION="${PROGRAM_VERSION}"
)
# Set C++ standard
set_property(TARGET ${PROGRAM_NAME} PROPERTY CXX_STANDARD 11)

# Enable testing
enable_testing()

# Add the test files
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_SOURCES ${TEST_DIR}/*.cpp)

# create the test executable
add_executable(${PROGRAM_NAME}_tests ${SOURCES} ${INCLUDE_DIR}/X11wrapper/mockX11Wrapper.hpp ${TEST_SOURCES} )

# Link against X11, cxxopts, GoogleTest, and GoogleMock libraries
target_link_libraries(${PROGRAM_NAME}_tests
        ${X11_LIBRARIES}
        cxxopts
        gtest
        gmock
        jsoncpp_lib
        gtest_main
        --coverage
)

# Set C++ standard for tests
set_property(TARGET ${PROGRAM_NAME}_tests PROPERTY CXX_STANDARD 17)
target_compile_options(${PROGRAM_NAME}_tests PRIVATE --coverage)

# Add the test as a target for running with 'make test'
add_test(NAME ${PROGRAM_NAME}_tests COMMAND ${PROGRAM_NAME}_tests)

add_library(clockWidget SHARED plugins/clockWidget/clock.cpp)
target_include_directories(clockWidget PRIVATE ${INCLUDE_DIR} ${XFT_INCLUDE_DIRS})
target_include_directories(clockWidget PRIVATE ${X11_INCLUDE_DIR})
link_directories(${XFT_LIBRARY_DIRS})
target_link_libraries(clockWidget ${X11_LIBRARIES} ${XFT_LIBRARIES})
add_definitions(${XFT_CFLAGS_OTHER})
set_target_properties(clockWidget PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
add_library(groupWidget SHARED plugins/groupWidget/groupw.cpp)
target_include_directories(groupWidget PRIVATE ${INCLUDE_DIR} ${XFT_INCLUDE_DIRS})
target_include_directories(groupWidget PRIVATE ${X11_INCLUDE_DIR})
link_directories(${XFT_LIBRARY_DIRS})
target_link_libraries(groupWidget ${X11_LIBRARIES} ${XFT_LIBRARIES})
add_definitions(${XFT_CFLAGS_OTHER})
set_target_properties(groupWidget PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)