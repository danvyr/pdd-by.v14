#include "DelphiRandom.h"

namespace PddBy
{

DelphiRandom::DelphiRandom(std::uint32_t randSeed) :
    m_randSeed(randSeed)
{
    //
}

void DelphiRandom::SetSeed(std::uint32_t randSeed)
{
    m_randSeed = randSeed;
}

std::uint32_t DelphiRandom::GetSeed() const
{
    return m_randSeed;
}

std::uint32_t DelphiRandom::GetNext(std::uint32_t limit)
{
    m_randSeed = (m_randSeed * 0x08088405ULL + 1);
    return (static_cast<std::uint64_t>(m_randSeed) * limit) >> 32;
}

} // namespace PddBy
