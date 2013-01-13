#include "RollingStonesRandom.h"

#include "Decode/Random/DelphiRandom.h"

namespace
{

std::uint32_t RotateLeft(std::uint32_t value, std::uint8_t shift)
{
    return (value << shift) | (value >> (sizeof(std::uint32_t) * 8 - shift));
}

std::uint32_t GetNextRandomInt32(PddBy::DelphiRandom& random)
{
    std::uint8_t const a = random.GetNext(256);
    std::uint8_t const b = random.GetNext(256);
    std::uint8_t const c = random.GetNext(256);
    std::uint8_t const d = random.GetNext(256);

    return a | (b << 8) | (c << 16) | (d << 24);
}

} // anonymous namespace

namespace PddBy
{

RollingStonesRandom::RollingStonesRandom(Buffer const& seed) :
    m_magic(4096, 0)
{
    std::uint32_t randSeed = 0;
    for (std::size_t i = seed.size(); i > 0; i--)
    {
        randSeed = seed[i - 1] ^ RotateLeft(randSeed, 8);
    }

    DelphiRandom random(randSeed);

    for (std::size_t i = 0; i < m_magic.size(); i++)
    {
        m_magic[i] = GetNextRandomInt32(random);
    }
}

std::uint8_t RollingStonesRandom::GetNext(std::size_t offset)
{
    return m_magic[offset % m_magic.size()];
}

} // namespace PddBy
