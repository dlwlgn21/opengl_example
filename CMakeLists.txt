cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME opengl_example)
project(${PROJECT_NAME})

# --- 추가: 파이썬 찾기 ---
find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_compile_options(/utf-8)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "OpenGL Example")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 580)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Common.cpp src/Common.h
    src/Shader.cpp src/Shader.h
    src/Program.cpp src/Program.h
    src/Context.cpp src/Context.h
    src/Buffer.cpp src/Buffer.h
    src/VertexLayout.cpp src/VertexLayout.h
    src/Image.h src/Image.cpp
    src/Texture.h src/Texture.cpp
    src/Mesh.h src/Mesh.cpp
)

# 파이썬 경로를 Dependency 쪽으로 넘겨줄 변수에 저장
set(PYTHON_EXE "${Python3_EXECUTABLE}")
include(Dependency.cmake)
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

add_dependencies(${PROJECT_NAME} ${DEP_LIST})
if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /wd4819)
endif()