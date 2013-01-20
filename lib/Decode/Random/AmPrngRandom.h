#pragma once

#include "Buffer.h"

#include <memory>

#include <stdint.h>

namespace PddBy
{

class AmPrngRandom
{
    class Impl;

public:
    AmPrngRandom(Buffer const& key, Buffer const& iv, std::size_t dropLength);
    ~AmPrngRandom();

    uint8_t GetNext();

private:
    std::auto_ptr<Impl> const m_impl;
};

} // namespace PddBy
