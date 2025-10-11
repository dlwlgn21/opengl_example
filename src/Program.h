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
private:
    Program() = default;
    bool TryLink(const std::vector<Shader*>& shaders);
    uint32_t mProgramId { 0 };
};


#endif // __PROGRAM_H__