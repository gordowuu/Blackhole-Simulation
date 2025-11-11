cmake_minimum_required(VERSION 3.15)
project(BlackholeSim VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Find OpenMP for CPU parallelization
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Add external dependencies
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

# GLAD
add_library(glad STATIC
    external/glad/src/glad.c
    external/glad/include/glad/glad.h
    external/glad/include/KHR/khrplatform.h
)
target_include_directories(glad PUBLIC external/glad/include)

# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw glad)

# stb_image
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE external/stb)

# Main executable source files
set(SOURCES
    src/main.cpp
    src/Core/Window.cpp
    src/Core/Shader.cpp
    src/Core/Camera.cpp
    src/Core/Input.cpp
    src/Physics/BlackHole.cpp
    src/Physics/AccretionDisk.cpp
    src/Rendering/Renderer.cpp
    src/Rendering/Texture.cpp
    src/Rendering/PostProcess.cpp
    src/UI/Interface.cpp
)

set(HEADERS
    src/Core/Window.h
    src/Core/Shader.h
    src/Core/Camera.h
    src/Core/Input.h
    src/Physics/BlackHole.h
    src/Physics/AccretionDisk.h
    src/Physics/Constants.h
    src/Rendering/Renderer.h
    src/Rendering/Texture.h
    src/Rendering/PostProcess.h
    src/UI/Interface.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm::glm
    imgui
    stb_image
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

# Platform-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    # Set subsystem to CONSOLE for debugging
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

# Copy shaders to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY shaders DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)
