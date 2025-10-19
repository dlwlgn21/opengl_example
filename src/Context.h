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
private:
    Context() = default;
    bool TryInit();
    std::unique_ptr<Program> mProgram;
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
};

#endif //__CONTEXT_H__