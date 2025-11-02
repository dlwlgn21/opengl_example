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
#include "FrameBuffer.h"
#include "CubeTexture.h"
#include "ShadowMap.h"

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
    FORCEINLINE void DrawImGui();
    FORCEINLINE void SetLightingProgram(const glm::mat4& view, const glm::mat4& projection, const Program* program) const;
    FORCEINLINE void DrawSkybox(const glm::mat4& view, const glm::mat4& projection) const;
    FORCEINLINE void DrawScene(const glm::mat4& view, const glm::mat4& projection, const Program* program) const;
    FORCEINLINE void DrawToFramebuffer(const Program* program) const;
    FORCEINLINE void DrawShadowMap(const Program* program);
private:
    Context() = default;
    bool TryInit();
    void InitPrograms();
    void InitMeshes();
    void InitMaterials();
    void InitTextures();
    void InitModels();
    void InitEtc();
    void InitBuffersAndLayouts();
    std::unique_ptr<Program> mDefaultLightingProgram;
    std::unique_ptr<Program> mSimpleColorProgram;
    std::unique_ptr<Program> mTextureProgram;
    std::unique_ptr<Program> mPostProgram;
    std::unique_ptr<Program> mSkyboxProgram;
    std::unique_ptr<Program> mEnvmapProgram;
    std::unique_ptr<Program> mGrassProgram;
    std::unique_ptr<Program> mLightingShadowProgram;


    float mGamma {1.0f};

    std::unique_ptr<Mesh> mBoxMesh;
    std::unique_ptr<Mesh> mPlaneMesh;


    std::unique_ptr<Texture> mWindowTexture;
    std::unique_ptr<Texture> mGrassTexture;
    std::unique_ptr<CubeTexture> mCubeTexture;
    std::unique_ptr<ShadowMap> mShadowMap;

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


    glm::highp_mat4 mLightView;
    glm::highp_mat4 mLightProjection;

    bool mIsBlinnShading { true };

    bool mIsFlashLight {};
    bool mIsCamControl {};
    glm::vec2 mPrevMousePos { glm::vec2(0.0f) };

    bool mIsAnimation { true };
    // light parameter
    struct Light
    {
        //glm::vec3 Direction { glm::vec3(-0.2f, -1.0f, -0.3f) };
        bool IsDirectional { true };
        glm::vec3 Pos { glm::vec3(2.0f, 4.0f, 4.0f) };
        glm::vec3 Dir { glm::vec3(-0.5f, -1.5f, -1.0f) };
        glm::vec2 Cutoff { glm::vec2(50.0f, 5.0f) }; 
        float Dist { 150.0f };
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

    std::unique_ptr<Framebuffer> mFramebuffer;
    std::unique_ptr<Buffer> mGrassPosBuffer;
    std::unique_ptr<VertexLayout> mGrassInstanceLayout;
    std::vector<glm::vec3> mGrassPositions;
};

#endif //__CONTEXT_H__