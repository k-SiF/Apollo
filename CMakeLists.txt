cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project(Apollo)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

add_library(glad STATIC thirdparty/glad/src/gl.c)
target_include_directories(glad PUBLIC thirdparty/glad/include)

# --- Engine library ---
add_library(apollo_engine
    engine/src/window.cpp
    engine/src/shader.cpp
    engine/src/mesh.cpp
    engine/src/entity.cpp
    engine/src/input.cpp
    engine/src/renderer.cpp
    engine/src/scene.cpp
    engine/src/sketch.cpp
    engine/src/entry.cpp
    engine/src/chrono.cpp
    engine/src/texture.cpp
    engine/src/camera.cpp)

target_include_directories(apollo_engine
    PUBLIC engine/include           # users see headers under here
    PRIVATE engine/include/apollo thirdparty/stb)  # engine's own .cpp can use short includes

target_link_libraries(apollo_engine PRIVATE glfw glad)
target_link_libraries(apollo_engine PUBLIC glm)
target_compile_definitions(apollo_engine PRIVATE GLFW_INCLUDE_NONE)
    
# --- Sandbox ---
file(GLOB SANDBOX_SOURCES CONFIGURE_DEPENDS sandbox/*.cpp)
add_executable(sandbox ${SANDBOX_SOURCES})
target_link_libraries(sandbox PRIVATE apollo_engine)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:sandbox>/shaders)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/sandbox/assets $<TARGET_FILE_DIR:sandbox>/assets)