#include "VertexLayout.h"

using namespace std;

unique_ptr<VertexLayout> VertexLayout::Create()
{
    unique_ptr<VertexLayout> vl = unique_ptr<VertexLayout>(new VertexLayout());
    vl->Init();
    return move(vl);
}
VertexLayout::~VertexLayout()
{
    if (mVertexLayoutObject)
    {
        glDeleteVertexArrays(1, &mVertexLayoutObject);
    }
}
void VertexLayout::Bind() const
{
    glBindVertexArray(mVertexLayoutObject);
}
void VertexLayout::SetAttrib(
    uint32_t attribIdx, int count,
    uint32_t type, bool isNormalized,
    size_t stride, uint64_t offset
) const
{
    glEnableVertexAttribArray(attribIdx);
    glVertexAttribPointer(attribIdx, count, type, isNormalized, stride, (const void*)offset);
}
void VertexLayout::DisableAttrib(int attribIdx) const
{

}

void VertexLayout::Init()
{
    glGenVertexArrays(1, &mVertexLayoutObject);
    Bind();
}