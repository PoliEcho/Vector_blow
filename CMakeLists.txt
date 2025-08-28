cmake_minimum_required(VERSION 3.20)

project(Vector_blow LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)


set(SDL3IMAGE_AVIF OFF)
set(SDL3IMAGE_INSTALL OFF)
set(SDL3IMAGE_BUILD_SHARED_LIBS OFF)


include(FetchContent)


FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(SDL3 SDL3_image)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wno-write-strings
    -Wsuggest-attribute=const
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3-static
    SDL3_image::SDL3_image-static
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -ggdb)
elseif(CMAKE_BUILD_TYPE STREQUAL "ASan")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address
        -ggdb
        -fno-omit-frame-pointer
    )
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()
