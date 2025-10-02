
include(ExternalProject)
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)
#set(DEP_COMMON_ARGS
#    -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
#    -DCMAKE_POLICY_VERSION_MINIMUM=3.5
#)
set(DEP_COMMON_ARGS
    -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    -DCMAKE_C_FLAGS=/utf-8
    -DCMAKE_CXX_FLAGS=/utf-8
    -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL
)
# spdlog
ExternalProject_Add(
    dep_spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.x"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        ${DEP_COMMON_ARGS}
        -DSPDLOG_BUILD_TESTS=OFF
    TEST_COMMAND ""
)
set(DEP_LIST ${DEP_LIST} dep_spdlog)
set(DEP_LIBS ${DEP_LIBS} spdlog$<$<CONFIG:Debug>:d>)

# glfw
ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.4"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        ${DEP_COMMON_ARGS}
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
)

set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)


# glad
ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG "v0.1.34"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        -DPython_EXECUTABLE=${PYTHON_EXE}   # ← 이 줄 추가 (중요)
        ${DEP_COMMON_ARGS}
        -DGLAD_INSTALL=ON
)

set(DEP_LIST ${DEP_LIST} dep_glad)
set(DEP_LIBS ${DEP_LIBS} glad)