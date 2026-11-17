cmake_minimum_required(VERSION 3.16)

project(Clawmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB GENERATED_SOURCES "generated/*.cpp" "generated/sprites/*.cpp")

add_executable(Clawmark
    src/main.cpp
    src/Scene.cpp
    src/platform/desktop/SDLPlatform.cpp  # TODO: Add other platform implementations in the future

    ${GENERATED_SOURCES}  # Your project!
)

target_link_libraries(Clawmark PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)
target_include_directories(Clawmark PRIVATE src/)