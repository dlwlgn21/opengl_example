#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Context.h"

using namespace std;

void OnFrameBufferSizeChanged(GLFWwindow* pWindow, int width, int height)
{
    SPDLOG_INFO("FrameBuffer Size Changed: ({} x {})", width, height);
    Context* context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(pWindow));
    assert(context != nullptr);
    context->Reshape(width, height);
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

void OnCursorPos(GLFWwindow* pWindow, double x, double y)
{
    Context* pContext = reinterpret_cast<Context*>(glfwGetWindowUserPointer(pWindow));
    assert(pContext != nullptr);
    pContext->OnMouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
  Context* pContext = (Context*)glfwGetWindowUserPointer(window);
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  pContext->OnMouseButton(button, action, x, y);
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
    unique_ptr<Context> context = Context::CreateOrNull();
    if (context == nullptr)
    {
        SPDLOG_ERROR("Failded To Create Context");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(pWindow, context.get());
    OnFrameBufferSizeChanged(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(pWindow, OnFrameBufferSizeChanged);
    glfwSetKeyCallback(pWindow, OnKeyEvent);
    glfwSetCursorPosCallback(pWindow, OnCursorPos);
    glfwSetMouseButtonCallback(pWindow, OnMouseButton);
    
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();
        context->ProcessInput(pWindow);
        context->Render();
        glfwSwapBuffers(pWindow);
    }
    context.reset();
    glfwTerminate();

    return 0;
}