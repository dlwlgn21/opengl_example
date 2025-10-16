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

    unique_ptr<Image> img2 = Image::LoadOrNull("./Image/awesomeface.png");
    if (img2 == nullptr)
    {
        return false;
    }
    
    unique_ptr<Image> img = Image::LoadOrNull("./Image/container.jpg");
    if (img == nullptr)
    {
        return false;
    }
    mTexture = Texture::CreateFromImg(img.get());
    mTexture2 = Texture::CreateFromImg(img2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture->GetId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture2->GetId());

    mProgram->Use();
    glUniform1i(glGetUniformLocation(mProgram->GetId(), "texSampler1"), 0);
    glUniform1i(glGetUniformLocation(mProgram->GetId(), "texSampler2"), 1);
    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    mProgram->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
