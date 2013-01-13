#pragma once

#include "Decode/IWriteStream.h"

namespace PddBy
{

class MemoryWriteStream : public IWriteStream
{
public:
    MemoryWriteStream(Buffer& buffer);
    virtual ~MemoryWriteStream();

    virtual void Write(Buffer const& buffer);
    virtual void Write(void const* buffer, std::size_t size);

private:
    Buffer& m_buffer;
};

typedef std::unique_ptr<IWriteStream> IWriteStreamPtr;

} // namespace PddBy
