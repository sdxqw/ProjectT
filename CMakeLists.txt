cmake_minimum_required(VERSION 3.30)
project(ProjectT C)

set(CMAKE_C_STANDARD 17)

# Set SDL2 paths based on the platform
if (WIN32)
    set(SDL2_BIN "bin/x64/")
    set(SDL2_LIB "lib/x64/")
    set(SDL2_INCLUDE "include/SDL2")
elseif (APPLE)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    set(SDL2_INCLUDE ${SDL2_INCLUDE_DIRS})
    set(SDL2_LIB ${SDL2_LIBRARIES})
elseif (UNIX)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    set(SDL2_INCLUDE ${SDL2_INCLUDE_DIRS})
    set(SDL2_LIB ${SDL2_LIBRARIES})
endif ()

include_directories(${SDL2_INCLUDE})
link_directories(${SDL2_LIB})

# Copy bin over to cmake build (Windows only)
if (WIN32)
    file(COPY ${SDL2_BIN} DESTINATION ${CMAKE_BINARY_DIR})
endif ()

# Include assets inside the exe
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(ProjectT src/main.c
        src/window.c
        src/window.h
        src/player.h
        src/player.c
        src/sprite_sheet.h
        src/sprite_sheet.c)

target_link_libraries(ProjectT SDL2main SDL2 SDL2_image)

# Set optimization flags
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

    # Set WIN32 subsystem for Windows to prevent console window
    if (WIN32)
        set_target_properties(ProjectT PROPERTIES WIN31_EXECUTABLE TRUE)
    endif ()
endif ()


