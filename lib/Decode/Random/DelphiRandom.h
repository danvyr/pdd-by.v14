#pragma once

#include <stdint.h>

namespace PddBy
{

class DelphiRandom
{
public:
    DelphiRandom(uint32_t randSeed = 0xccba8e81);

    void SetSeed(uint32_t randSeed);
    uint32_t GetSeed() const;

    uint32_t GetNext(uint32_t limit);

private:
    uint32_t m_randSeed;
};

} // namespace PddBy
