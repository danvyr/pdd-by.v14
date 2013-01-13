#pragma once

#include "Decode/IFilter.h"

namespace PddBy
{

class A8ImageFilter : public IFilter
{
public:
    A8ImageFilter(std::string const& imageMame, std::uint16_t magicNumber);

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    std::uint32_t const m_randomSeed;
};

} // namespace PddBy
