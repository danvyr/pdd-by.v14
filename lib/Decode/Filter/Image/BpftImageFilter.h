#pragma once

#include "Decode/IFilter.h"

#include <string>

namespace PddBy
{

class BpftImageFilter : public IFilter
{
public:
    BpftImageFilter(std::string const& imageName, uint16_t magicNumber, uint16_t magicSeed = 0);
    virtual ~BpftImageFilter();

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    std::string const m_imageName;
    uint16_t const m_magicNumber;
    uint16_t const m_magicSeed;
};

} // namespace PddBy
