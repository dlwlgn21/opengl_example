#ifndef __VERTEX_LAYOUT_H__
#define __VERTEX_LAYOUT_H__

#include "Common.h"

CLASS_PTR(VertexLayout)
class VertexLayout
{
public:
    static std::unique_ptr<VertexLayout> Create();
    ~VertexLayout();
    uint32_t GetId() const { return mVertexLayoutObject; }
    void Bind() const;
    void SetAttrib(
        uint32_t attribIdx, int count,
        uint32_t type, bool isNormalized,
        size_t stride, uint64_t offset
    ) const;
    void DisableAttrib(int attribIdx) const; 
private:
    VertexLayout() = default;
    void Init();
    uint32_t mVertexLayoutObject;
};

#endif //__VERTEX_LAYOUT_H__