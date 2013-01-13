#include "MemoryReadStream.h"

#include "Decode/Stream/StreamShit.h"

namespace PddBy
{

MemoryReadStream::MemoryReadStream(Buffer const& buffer) :
    m_buffer(buffer)
{
    //
}

MemoryReadStream::MemoryReadStream(Buffer&& buffer) :
    m_buffer(std::move(buffer))
{
    //
}

MemoryReadStream::~MemoryReadStream()
{
    //
}

void MemoryReadStream::Read(Buffer& buffer, std::size_t size)
{
    buffer.resize(size);
    Read(&buffer[0], size);
}

void MemoryReadStream::Read(void* buffer, std::size_t size)
{
    if (m_buffer.size() < size)
    {
        throw EndOfStreamShit("Premature end of data");
    }

    std::copy(&m_buffer[0], &m_buffer[size], static_cast<std::uint8_t*>(buffer));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + size);
}

void MemoryReadStream::ReadToEnd(Buffer& buffer)
{
    buffer.clear();
    buffer.swap(m_buffer);
}

} // namespace PddBy
