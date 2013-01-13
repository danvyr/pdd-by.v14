#include "MemoryWriteStream.h"

namespace PddBy
{

MemoryWriteStream::MemoryWriteStream(Buffer& buffer) :
    m_buffer(buffer)
{
    //
}

MemoryWriteStream::~MemoryWriteStream()
{
    //
}

void MemoryWriteStream::Write(Buffer const& buffer)
{
    Write(&buffer[0], buffer.size());
}

void MemoryWriteStream::Write(void const* buffer, std::size_t size)
{
    std::copy(static_cast<std::uint8_t const*>(buffer), static_cast<std::uint8_t const*>(buffer) + size,
        std::back_inserter(m_buffer));
}

} // namespace PddBy
