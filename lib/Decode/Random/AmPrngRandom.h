#pragma once

#include "pddby/Buffer.h"

#include <memory>

namespace PddBy
{

class AmPrngRandom
{
    class Impl;

public:
    AmPrngRandom(Buffer const& key, Buffer const& iv, std::size_t dropLength);
    ~AmPrngRandom();

    std::uint8_t GetNext();

private:
    std::unique_ptr<Impl> m_impl;
};

} // namespace PddBy
