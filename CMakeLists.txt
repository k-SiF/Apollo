cmake_minimum_required(VERSION 3.20)
project(Apollo)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC thirdparty/glad/src/gl.c)
target_include_directories(glad PUBLIC thirdparty/glad/include)

add_executable(apollo src/main.cpp src/shader.cpp src/mesh.cpp)
target_link_libraries(apollo PRIVATE glfw glad)

add_custom_command(TARGET apollo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:apollo>/shaders)