#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Common.h"
#include "Shader.h"
#include "Program.h"
#include "Buffer.h"
#include "VertexLayout.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"

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
    std::unique_ptr<Mesh> mBoxMesh;
    std::unique_ptr<Texture> mTexture;
    std::unique_ptr<Texture> mTexture2;
    std::unique_ptr<Model> mBagModel;
    std::unique_ptr<Material> mPlaneMaterial;
    std::unique_ptr<Material> mBox1Material;
    std::unique_ptr<Material> mBox2Material;
    std::unique_ptr<Material> mBagMaterial;
    uint32_t mVertexArrayObject;

    // Cam Param
    glm::vec3 mCamPos { glm::vec3(0.0f, 2.5f, 8.0f) };
    glm::vec3 mCamFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 mCamUp {glm::vec3(0.0f, 1.0f, 0.0f)};
    float mCamPitch { -20.0f };
    float mCamYaw { };

    // Viewport
    int mWidth { WINDOW_WIDTH };
    int mHeight { WINDOW_HEIGHT };

    glm::vec4 mClearColor { glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };

    bool mIsFlashLight {};
    bool mIsCamControl {};
    glm::vec2 mPrevMousePos { glm::vec2(0.0f) };

    bool mIsAnimation { true };
    // light parameter
    struct Light
    {
        //glm::vec3 Direction { glm::vec3(-0.2f, -1.0f, -0.3f) };
        glm::vec3 Pos { glm::vec3(1.0f, 4.0f, 4.0f) };
        glm::vec3 Dir { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2 Cutoff { glm::vec2(120.0f, 5.0f) }; 
        float Dist { 128.0f };
        glm::vec3 Ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 Diffuse { glm::vec3(0.8f, 0.8f, 0.8f) };
        glm::vec3 Specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light mLight;

    // material parameter
    // struct Material
    // {
    //     std::unique_ptr<Texture> Diffuse;
    //     std::unique_ptr<Texture> Specular;
    //     float Shininess { 32.0f };
    // };
};

#endif //__CONTEXT_H__