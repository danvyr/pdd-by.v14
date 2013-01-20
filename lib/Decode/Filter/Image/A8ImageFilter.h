#pragma once

#include "Decode/IFilter.h"

#include <string>

#include <stdint.h>

namespace PddBy
{

class A8ImageFilter : public IFilter
{
public:
    A8ImageFilter(std::string const& imageMame, uint16_t magicNumber);
    virtual ~A8ImageFilter();

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    uint32_t const m_randomSeed;
};

} // namespace PddBy
