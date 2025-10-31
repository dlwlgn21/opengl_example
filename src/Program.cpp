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
    SPDLOG_INFO("[Program Id {}", program->GetId());
    SPDLOG_INFO("VertexShader Id {}", shaders[0]->GetId());
    SPDLOG_INFO("FragmentShader Id {}]", shaders[1]->GetId());
    return move(program);
}

unique_ptr<Program> Program::CreateOrNull(const std::string& vsFilename, const std::string& fsFilename)
{
  unique_ptr<Shader> vs = Shader::CreateFromFileOrNull(vsFilename, GL_VERTEX_SHADER);
  unique_ptr<Shader> fs = Shader::CreateFromFileOrNull(fsFilename, GL_FRAGMENT_SHADER);
  
  if (vs == nullptr || fs == nullptr)
  {
      return nullptr;
  }


  return std::move(CreateOrNull({vs.get(), fs.get()}));
}

void Program::SetUniform(const std::string& name, const glm::vec4& value) const
{
  auto loc = glGetUniformLocation(mProgramId, name.c_str());
  glUniform4fv(loc, 1, glm::value_ptr(value));
}
Program::~Program()
{
    if (mProgramId)
    {
        glDeleteProgram(mProgramId);
    }
}

void Program::SetUniform(const std::string& name, int value) const
{
    GLint location = glGetUniformLocation(mProgramId, name.c_str());
    glUniform1i(location, value);
}
void Program::SetUniform(const std::string& name, const glm::mat4& value) const
{
    GLint location = glGetUniformLocation(mProgramId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Program::SetUniform(const std::string& name, const float value) const
{
    GLint location = glGetUniformLocation(mProgramId, name.c_str());
    glUniform1f(location, value);

}
void Program::SetUniform(const std::string& name, const glm::vec2& value) const
{
    GLint location = glGetUniformLocation(mProgramId, name.c_str());
    glUniform2fv(location, 1, glm::value_ptr(value));
}
void Program::SetUniform(const std::string& name, const glm::vec3& value) const
{
    GLint location = glGetUniformLocation(mProgramId, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
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
void Program::Use() const
{
    glUseProgram(mProgramId);
}