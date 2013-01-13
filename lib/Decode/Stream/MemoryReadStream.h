#pragma once

#include "Decode/IReadStream.h"

namespace PddBy
{

class MemoryReadStream : public IReadStream
{
public:
    MemoryReadStream(Buffer const& buffer);
    MemoryReadStream(Buffer&& buffer);
    virtual ~MemoryReadStream();

    // IReadStream implementation
    virtual void Read(Buffer& buffer, std::size_t size);
    virtual void Read(void* buffer, std::size_t size);
    virtual void ReadToEnd(Buffer& buffer);

private:
    Buffer m_buffer;
};

} // namespace PddBy
