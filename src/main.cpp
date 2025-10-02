#include "Common.h"


void OnFrameBufferSizeChanged(GLFWwindow* pWindow, int width, int height)
{
    SPDLOG_INFO("FrameBuffer Size Changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* pWindow, int key, int scanCode, int action, int mods)
{
    SPDLOG_INFO("key: {}, scanCode {}, action : {}, mods: {}{}{}", 
        key, 
        scanCode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-"
    );
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
}




int main(int argc, const char** argv)
{
    SPDLOG_INFO("Start Program");

    SPDLOG_INFO("Init glfw");
    if (!glfwInit())
    {
        const char* pDesc = nullptr;
        glfwGetError(&pDesc);
        SPDLOG_ERROR("Failded To Init glfw: {}", pDesc);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SPDLOG_INFO("Create glfw window");
    GLFWwindow* pWindow = glfwCreateWindow(
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        WINDOW_NAME,
        nullptr,
        nullptr
    );

    if (pWindow == nullptr)
    {
        SPDLOG_ERROR("Failded To create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_ERROR("failded to init glad");
        glfwTerminate();
        return -1;
    }

    const GLubyte* pGlVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL Context version : {}", reinterpret_cast<const char*>(pGlVersion));

    OnFrameBufferSizeChanged(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(pWindow, OnFrameBufferSizeChanged);
    glfwSetKeyCallback(pWindow, OnKeyEvent);
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(pWindow);
    }

    glfwTerminate();

    return 0;
}