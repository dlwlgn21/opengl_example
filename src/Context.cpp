#include "Context.h"

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
        0.5f, 0.5f, 0.0f, // TopRight
        0.5f, -0.5f, 0.0f, // BotRight
        -0.5f, -0.5f, 0.0f, // BotLeft
        -0.5f, 0.5f, 0.0f // TopLeft
    };
    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer); // GL_ARRAY_BUFFER -> VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);

    unique_ptr<Shader> vs = Shader::CreateFromFileOrNull("./Shader/Simple.vs", GL_VERTEX_SHADER);
    unique_ptr<Shader> fs = Shader::CreateFromFileOrNull("./Shader/Simple.fs", GL_FRAGMENT_SHADER);
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
    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mProgram->GetId());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
