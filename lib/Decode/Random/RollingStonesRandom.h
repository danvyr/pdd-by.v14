#pragma once

#include "Buffer.h"

namespace PddBy
{

class RollingStonesRandom
{
public:
    RollingStonesRandom(Buffer const& seed);

    uint8_t GetNext(std::size_t offset);

private:
    Buffer m_magic;
};

} // namespace PddBy
