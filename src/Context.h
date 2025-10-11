#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Common.h"
#include "Shader.h"
#include "Program.h"
#include "Buffer.h"

CLASS_PTR(Context)
class Context
{
public:
    static std::unique_ptr<Context> CreateOrNull();
    void Render();

private:
    Context() = default;
    bool TryInit();
    std::unique_ptr<Program> mProgram;
    std::unique_ptr<Buffer> mVertexBuffer;
    std::unique_ptr<Buffer> mIndexBuffer;
    uint32_t mVertexArrayObject;
};

#endif //__CONTEXT_H__