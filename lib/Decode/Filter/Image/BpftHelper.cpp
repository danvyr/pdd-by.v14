#include "BpftHelper.h"

namespace PddBy
{

namespace Magic
{

std::uint16_t const Salt = 0xa001;

} // namespace Magic

std::uint32_t BpftHelper::ImageNameToRandSeed(std::string const& name, std::uint16_t magicNumber)
{
    std::uint16_t seed = magicNumber;
    for (std::size_t i = 0; i < name.size(); i++)
    {
        std::uint8_t ch = std::toupper(name[i]);
        for (std::size_t j = 0; j < 8; j++)
        {
            std::uint64_t const oldSeed = seed;
            seed >>= 1;
            if ((ch ^ oldSeed) & 1)
            {
                seed ^= Magic::Salt;
            }

            ch >>= 1;
        }
    }

    return seed;
}

} // namespace PddBy
