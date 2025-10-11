#include "Shader.h"

ShaderUPtr Shader::CreateFromFileOrNull(const std::string& filename, GLenum shaderType)
{
    ShaderUPtr uptrShader = ShaderUPtr(new Shader());

    if (!uptrShader->TryLoadFile(filename, shaderType))
    {
        return nullptr;
    }
    return std::move(uptrShader);
}

Shader::~Shader()
{
    if (mShaderId)
    {
        glDeleteShader(mShaderId);
    }
}

bool Shader::TryLoadFile(const std::string& filename, GLenum shaderType)
{
    std::optional<std::string> result = LoadTextFileOrEmpty(filename);
    if (!result.has_value())
    {
        return false;
    }
    std::string& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    // Create And Compile Shader
    mShaderId = glCreateShader(shaderType);
    glShaderSource(mShaderId, 1, (const GLchar* const*)&codePtr, &codeLength);
    glCompileShader(mShaderId);

    // Check Compile Error
    int success = 0;
    glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(mShaderId, 1024, nullptr, infoLog);
        SPDLOG_ERROR("Failed to compile Shader : \"{}\"", filename);
        SPDLOG_ERROR("Reason : {}", infoLog);
        return false;
    }
    return true;
}
