#pragma once

#include <string>

#include <stdint.h>

namespace PddBy
{

class BpftHelper
{
public:
    static uint32_t ImageNameToRandSeed(std::string const& name, uint16_t magicNumber);
};

} // namespace PddBy
