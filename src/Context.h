#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Common.h"
#include "Shader.h"
#include "Program.h"
#include "Buffer.h"
#include "VertexLayout.h"
#include "Texture.h"

CLASS_PTR(Context)
class Context
{
public:
    static std::unique_ptr<Context> CreateOrNull();
    void Render();
    void ProcessInput(GLFWwindow* pWindow);
    void Reshape(int width, int height);

    void OnMouseMove(double x, double y);
    void OnMouseButton(int btn, int action, double x, double y);

private:
    Context() = default;
    bool TryInit();
    std::unique_ptr<Program> mProgram;
    std::unique_ptr<Program> mSimpleProgram;
    std::unique_ptr<Buffer> mVertexBuffer;
    std::unique_ptr<Buffer> mIndexBuffer;
    std::unique_ptr<VertexLayout> mVertexLayout;
    std::unique_ptr<Texture> mTexture;
    std::unique_ptr<Texture> mTexture2;
    uint32_t mVertexArrayObject;

    // Cam Param
    glm::vec3 mCamPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 mCamFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 mCamUp {glm::vec3(0.0f, 1.0f, 0.0f)};
    float mCamPitch {};
    float mCamYaw {};

    // Viewport
    int mWidth { WINDOW_WIDTH };
    int mHeight { WINDOW_HEIGHT };

    glm::vec4 mClearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

    bool mIsCamControl {};
    glm::vec2 mPrevMousePos { glm::vec2(0.0f) };

    bool mIsAnimation { true };
    // light parameter
    struct Light
    {
        glm::vec3 WorldPos { glm::vec3(3.0f, 3.0f, 3.0f) };
        glm::vec3 Ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 Diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3 Specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light mLight;

    // material parameter
    struct Material
    {
        std::unique_ptr<Texture> Diffuse;
        glm::vec3 Ambient { glm::vec3(1.0f, 0.5f, 0.3f) };
        glm::vec3 Specular { glm::vec3(0.5f, 0.5f, 0.5f) };
        float Shininess { 32.0f };
    };
    Material mMaterial;
};

#endif //__CONTEXT_H__