#include "BaseFilteringReadStream.h"

namespace PddBy
{

BaseFilteringReadStream::BaseFilteringReadStream(IReadStreamPtr stream) :
    m_stream(std::move(stream))
{
    //
}

BaseFilteringReadStream::~BaseFilteringReadStream()
{
    //
}

void BaseFilteringReadStream::Read(Buffer& buffer, std::size_t size)
{
    m_stream->Read(buffer, size);
    ApplyFilter(&buffer[0], buffer.size());
}

void BaseFilteringReadStream::Read(void* buffer, std::size_t size)
{
    m_stream->Read(buffer, size);
    ApplyFilter(static_cast<std::uint8_t*>(buffer), size);
}

void BaseFilteringReadStream::ReadToEnd(Buffer& buffer)
{
    m_stream->ReadToEnd(buffer);
    ApplyFilter(&buffer[0], buffer.size());
}

} // namespace PddBy
