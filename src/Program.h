#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "Common.h"
#include "Shader.h"

CLASS_PTR(Program)

class Program
{
public:
    static std::unique_ptr<Program> CreateOrNull(const std::vector<Shader*>& shaders);
    ~Program();
    uint32_t GetId() const { return mProgramId; }
    void Use() const;

    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

    void SetUniform(const std::string& name, const float value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;


private:
    Program() = default;
    bool TryLink(const std::vector<Shader*>& shaders);
    uint32_t mProgramId { 0 };
};


#endif // __PROGRAM_H__