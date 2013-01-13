#pragma once

#include "Decode/IFilter.h"

#include <string>

namespace PddBy
{

class BpftImageFilter : public IFilter
{
public:
    BpftImageFilter(std::string const& imageName, std::uint16_t magicNumber, std::uint16_t magicSeed = 0);

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    std::string const m_imageName;
    std::uint16_t const m_magicNumber;
    std::uint16_t const m_magicSeed;
};

} // namespace PddBy
