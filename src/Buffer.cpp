#include "Buffer.h"

using namespace std;

unique_ptr<Buffer> Buffer::CreateWithDataOrNull(uint32_t bufferType, uint32_t usage, const void* pData, size_t dataSize)
{
    unique_ptr<Buffer> buffer = unique_ptr<Buffer>(new Buffer());
    if (!buffer->TryInit(bufferType, usage, pData, dataSize))
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

bool Buffer::TryInit(uint32_t bufferType, uint32_t usage, const void* pData, size_t dataSize)
{
    mBufferType = bufferType;
    mUsage = usage;
    glGenBuffers(1, &mBuffer);
    Bind();
    glBufferData(mBufferType, dataSize, pData, usage);
    return true;
}
