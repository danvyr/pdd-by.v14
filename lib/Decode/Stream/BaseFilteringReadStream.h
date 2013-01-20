#pragma once

#include "Decode/IReadStream.h"

namespace PddBy
{

class BaseFilteringReadStream : public IReadStream
{
public:
    BaseFilteringReadStream(IReadStreamPtr stream);
    virtual ~BaseFilteringReadStream();

    virtual void Read(Buffer& buffer, std::size_t size);
    virtual void Read(void* buffer, std::size_t size);
    virtual void ReadToEnd(Buffer& buffer);

protected:
    virtual void ApplyFilter(uint8_t* buffer, std::size_t size) = 0;

private:
    IReadStreamPtr m_stream;
};

} // namespace PddBy
