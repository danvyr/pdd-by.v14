#pragma once

#include "pddby/Buffer.h"

#include <memory>

namespace PddBy
{

class IWriteStream
{
public:
    virtual ~IWriteStream() {}

    virtual void Write(Buffer const& buffer) = 0;
    virtual void Write(void const* buffer, std::size_t size) = 0;
};

typedef std::unique_ptr<IWriteStream> IWriteStreamPtr;

} // namespace PddBy
