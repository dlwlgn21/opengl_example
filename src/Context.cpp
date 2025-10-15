#include "Context.h"
#include "Image.h"

using namespace std;

unique_ptr<Context> Context::CreateOrNull()
{
    unique_ptr<Context> context = unique_ptr<Context>(new Context());
    if (!context->TryInit())
    {
        return nullptr;
    }
    return move(context);
}

bool Context::TryInit()
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // TopRight
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // BotRight
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// BotLeft
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f// TopLeft
    };
    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    mVertexLayout = VertexLayout::Create();

    mVertexBuffer = Buffer::CreateWithDataOrNull(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);

    mVertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    mVertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    mVertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    mIndexBuffer = Buffer::CreateWithDataOrNull(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    unique_ptr<Shader> vs = Shader::CreateFromFileOrNull("./Shader/Texture.vs", GL_VERTEX_SHADER);
    unique_ptr<Shader> fs = Shader::CreateFromFileOrNull("./Shader/Texture.fs", GL_FRAGMENT_SHADER);
    if (vs == nullptr || fs == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("VertexShader Id {}", vs->GetId());
    SPDLOG_INFO("FragmentShader Id {}", fs->GetId());
    mProgram = Program::CreateOrNull({fs.get(), vs.get()});
    if (mProgram == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("Program Id : {}", mProgram->GetId());
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    unique_ptr<Image> img = Image::LoadOrNull("./Image/container.jpg");
    if (img == nullptr)
    {
        return false;
    }
    SPDLOG_INFO("Image {}x, {}y, {}chnnels", 
        img->GetWidth(), 
        img->GetHeight(), 
        img->GetChannelCount()
    );

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // x coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // y coord
    // CPU -> GPU 복사
    glTexImage2D(
        GL_TEXTURE_2D,      // Target
        0,                  // GPU 쪽의 텍스쳐 데이터 기술 - Mip-Map 레벨값
        GL_RGB,             // GPU 쪽의 텍스쳐 데이터 기술 - GPU 쪽에서 유지 하려고 하는 채널의 타입
        img->GetWidth(),    // GPU 쪽의 텍스쳐 데이터 기술
        img->GetHeight(),   // GPU 쪽의 텍스쳐 데이터 기술
        0,                  // GPU 쪽의 텍스쳐 데이터 기술 - Border size
        GL_RGB,             // CPU 할당된 이미지 데이터 기술 - image의 픽셀 타입. 채널
        GL_UNSIGNED_BYTE,   // CPU 할당된 이미지 데이터 기술 - image가 하나의 채널을 표현하는데 쓰는 데이터 타입
        img->GetData()      // CPU 할당된 이미지 데이터 기술 - 실제 데이터가 들어가 있는 포인터 넘겨줌
    );

    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    mProgram->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
