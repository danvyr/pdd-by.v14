#include "DelphiRandom.h"

namespace PddBy
{

DelphiRandom::DelphiRandom(uint32_t randSeed) :
    m_randSeed(randSeed)
{
    //
}

void DelphiRandom::SetSeed(uint32_t randSeed)
{
    m_randSeed = randSeed;
}

uint32_t DelphiRandom::GetSeed() const
{
    return m_randSeed;
}

uint32_t DelphiRandom::GetNext(uint32_t limit)
{
    m_randSeed = (m_randSeed * 0x08088405ULL + 1);
    return (static_cast<uint64_t>(m_randSeed) * limit) >> 32;
}

} // namespace PddBy
