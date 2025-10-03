#ifndef __SHADER_H__
#define __SHADER_H__

#include "Common.h"

CLASS_PTR(Shader);

class Shader
{
public:
    static ShaderUPtr CreateFromFileOrNull(const std::string& filename, GLenum shaderType);

    ~Shader();
    uint32_t Get() const {return m_shader;}
private:
    Shader() {}
    bool LoadFile(const std::string& filename, GLenum shaderType);
    uint32_t m_shader { 0 };
};

#endif // __SHADER_H__