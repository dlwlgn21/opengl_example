#include "Buffer.h"

using namespace std;

unique_ptr<Buffer> Buffer::CreateWithDataOrNull(uint32_t bufferType, uint32_t usage, const void* pData, size_t stride, size_t count)
{
    unique_ptr<Buffer> buffer = unique_ptr<Buffer>(new Buffer());
    if (!buffer->TryInit(bufferType, usage, pData, stride, count))
    {
        return nullptr;
    }
    return move(buffer);
}

Buffer::~Buffer()
{
    if (mBuffer)
    {
        glDeleteBuffers(1, &mBuffer);
    }
}

void Buffer::Bind() const
{
    glBindBuffer(mBufferType, mBuffer);
}

bool Buffer::TryInit(uint32_t bufferType, uint32_t usage, const void* pData, size_t stride, size_t count)
{
    mBufferType = bufferType;
    mUsage = usage;
    mStride = stride;
    mCount = count;
    glGenBuffers(1, &mBuffer);
    Bind();
    glBufferData(mBufferType, mStride * mCount, pData, usage);
    return true;
}
