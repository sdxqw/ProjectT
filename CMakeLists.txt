cmake_minimum_required(VERSION 3.30)
project(ProjectT C)

set(CMAKE_C_STANDARD 17)

# Include assets inside the exe
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(ProjectT src/main.c
        src/window.c
        src/window.h
        src/player.h
        src/player.c
        src/sprite_sheet.h
        src/sprite_sheet.c
        src/world.h
        src/world.c
        src/camera.h
        src/camera.c
)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME SDL2
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-2.30.10
        GIT_SHALLOW TRUE
)
CPMAddPackage(
        NAME SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
        GIT_SHALLOW TRUE
)

# Include directories for SDL2 and SDL2_image
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(ProjectT SDL2::SDL2 SDL2_image::SDL2_image
        SDL2::SDL2main
)

# Set optimization flags
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

    # Set WIN32 subsystem for Windows to prevent console window
    if (WIN32)
        set_target_properties(ProjectT PROPERTIES WIN31_EXECUTABLE TRUE)
    endif ()
endif ()


