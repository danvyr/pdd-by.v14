#pragma once

#include "Decode/IFilter.h"

#include <string>

namespace PddBy
{

class BpftcamImageFilter : public IFilter
{
public:
    enum CipherType
    {
        TrashCartPrng,
        AmPrng
    };

public:
    BpftcamImageFilter(std::string const& imageName, std::uint16_t magicNumber, CipherType cipherType);

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    std::string const m_imageName;
    std::uint16_t const m_magicNumber;
    CipherType const m_cipherType;
};

} // namespace PddBy
