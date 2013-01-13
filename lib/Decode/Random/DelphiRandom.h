#pragma once

#include <cstdint>

namespace PddBy
{

class DelphiRandom
{
public:
    DelphiRandom(std::uint32_t randSeed = 0xccba8e81);

    void SetSeed(std::uint32_t randSeed);
    std::uint32_t GetSeed() const;

    std::uint32_t GetNext(std::uint32_t limit);

private:
    std::uint32_t m_randSeed;
};

} // namespace PddBy
