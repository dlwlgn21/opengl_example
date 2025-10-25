#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "Common.h"
CLASS_PTR(Buffer)
class Buffer
{
public:
    static std::unique_ptr<Buffer> CreateWithDataOrNull(uint32_t bufferType, uint32_t usage, const void* pData, size_t stride, size_t count);
    ~Buffer();
    
    uint32_t GetId() const { return mBuffer; }
    size_t GetStride() const { return mStride; }
    size_t GetCount() const { return mCount; }
    void Bind() const;
private:
    Buffer() = default;
    bool TryInit(uint32_t bufferType, uint32_t usage, const void* pData, size_t stride, size_t count);

    uint32_t mBuffer{};
    uint32_t mBufferType{};
    uint32_t mUsage{};
    size_t mStride {};
    size_t mCount {};
};


#endif //__BUFFER_H__