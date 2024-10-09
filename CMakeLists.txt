cmake_minimum_required(VERSION 3.29)
project(SpinningCube)

set(CMAKE_CXX_STANDARD 23)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -Wall -g")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
endif()

# Shader files
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/res/shaders/*")

# Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/dep/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
set(IMGUI_SRC
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/dep/inc)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/dep/lib)

# Copy shaders
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res/shaders)

# Add executables
add_executable(SpinningCube
        # Dependencies
        ${IMGUI_SRC}
        ${CMAKE_SOURCE_DIR}/src/glad.c
        # Application
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        src/gui/gui.cpp
        src/gui/gui.h
        src/app/appContext.h
        src/renderContext/renderContext.cpp
        src/renderContext/renderContext.h
        src/glew_glfw.h
        src/scene/scene.cpp
        src/scene/scene.h
        src/camera/baseCamera.cpp
        src/camera/baseCamera.h
        src/camera/CameraGameLike.cpp
        src/camera/CameraGameLike.h
        src/camera/CameraAnchorFree.cpp
        src/camera/CameraAnchorFree.h
        src/framebufferManager/FrameBufferManager.cpp
        src/framebufferManager/FrameBufferManager.h
        src/inputHandler/InputHandler.cpp
        src/inputHandler/InputHandler.h
        src/gridModule/gridModule.cpp
        src/gridModule/gridModule.h
        src/shader/shaderProgram.cpp
        src/VariantUtil.h
        src/objectsOpenGL/vertex.h
        src/objectsOpenGL/VBO.h
        src/objectsOpenGL/VBO.cpp
        src/objectsOpenGL/VAO.h
        src/objectsOpenGL/VAO.cpp
        src/objectsOpenGL/EBO.h
        src/objectsOpenGL/EBO.cpp
        src/mesh/mesh.h
        src/mesh/mesh.cpp
)

# Link libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")             # macOS
    target_link_libraries(SpinningCube
            ${CMAKE_SOURCE_DIR}/dep/lib/libglfw.3.4.dylib
            ${CMAKE_SOURCE_DIR}/dep/lib/libGLEW.2.2.0.dylib
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework CoreFoundation"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")        # Windows
    find_package(OpenGL REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(SpinningCube
            ${CMAKE_SOURCE_DIR}/dep/lib/glfw3.lib
            ${CMAKE_SOURCE_DIR}/dep/lib/glew32.lib
            ${OPENGL_LIBRARIES}
    )
endif()
