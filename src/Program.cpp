#include "Program.h"

using namespace std;

unique_ptr<Program> Program::CreateOrNull(const vector<Shader*>& shaders)
{
    unique_ptr<Program> program = unique_ptr<Program>(new Program());
    //unique_ptr<Program> program = make_unique<Program>();
    if (!program->TryLink(shaders))
    {
        return nullptr;
    }
    return move(program);
}

Program::~Program()
{
    if (mProgramId)
    {
        glDeleteProgram(mProgramId);
    }
}

bool Program::TryLink(const vector<Shader*>& shaders)
{
    mProgramId = glCreateProgram();
    for (auto* pShader : shaders)
    {
        glAttachShader(mProgramId, pShader->GetId());
    }
    glLinkProgram(mProgramId);

    int success = 0;
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(mProgramId, 1024, nullptr, infoLog);
        SPDLOG_ERROR("Failed to Link Program : {}", infoLog);
        return false;
    }
    return true;
}
