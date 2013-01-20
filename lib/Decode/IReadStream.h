#pragma once

#include "Buffer.h"

#include <memory>

namespace PddBy
{

class IReadStream
{
public:
    virtual ~IReadStream() {}

    virtual void Read(Buffer& buffer, std::size_t size) = 0;
    virtual void Read(void* buffer, std::size_t size) = 0;
    virtual void ReadToEnd(Buffer& buffer) = 0;
};

typedef std::auto_ptr<IReadStream> IReadStreamPtr;

class IRandomAccessReadStream : public IReadStream
{
public:
    virtual ~IRandomAccessReadStream() {}

    virtual std::size_t GetPosition() = 0;
    virtual void SetPosition(std::size_t offset) = 0;
};

typedef std::auto_ptr<IRandomAccessReadStream> IRandomAccessReadStreamPtr;

} // namespace PddBy
