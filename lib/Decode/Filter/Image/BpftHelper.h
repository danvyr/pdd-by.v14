#pragma once

#include <cstdint>
#include <string>

namespace PddBy
{

class BpftHelper
{
public:
    static std::uint32_t ImageNameToRandSeed(std::string const& name, std::uint16_t magicNumber);
};

} // namespace PddBy
