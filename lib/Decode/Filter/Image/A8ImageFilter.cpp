#include "A8ImageFilter.h"

#include "Decode/IReadStream.h"
#include "Decode/Random/DelphiRandom.h"
#include "Decode/Stream/MemoryReadStream.h"
#include "Shit.h"

#include <cassert>
#include <cctype>

namespace
{

struct __attribute__((packed)) ImageHeader
{
    // file header
    uint16_t Signature;
    uint32_t FileSize;
    uint16_t Reserved[2];
    uint32_t BitmapOffset;
    // information header
    uint32_t HeaderSize;
    uint32_t ImageWidth;
    uint32_t ImageHeight;
    uint16_t Planes;
    uint16_t Bpp;
    // ... not important
};

uint32_t ImageNameToRandSeed(std::string const& name, uint16_t magicNumber)
{
    uint32_t seed = 0;
    for (std::size_t i = 0; i < name.size(); i++)
    {
        if (std::isdigit(name[i]))
        {
            seed = seed * 10 + (name[i] - '0');
        }
    }

    return seed + magicNumber;
}

} // anonymous namespace

namespace PddBy
{

namespace Magic
{

std::string const FileHeader = "A8";

} // namespace Magic

A8ImageFilter::A8ImageFilter(std::string const& imageMame, uint16_t magicNumber) :
    m_randomSeed(ImageNameToRandSeed(imageMame, magicNumber))
{
    //
}

A8ImageFilter::~A8ImageFilter()
{
    //
}

IReadStreamPtr A8ImageFilter::Apply(IReadStreamPtr stream)
{
    Buffer buffer;
    stream->ReadToEnd(buffer);

    if (buffer.size() < Magic::FileHeader.size() ||
        std::string(reinterpret_cast<char const*>(&buffer[0]), Magic::FileHeader.size()) != Magic::FileHeader)
    {
        throw Shit("Image is not a valid A8 image");
    }

    ImageHeader& header = *reinterpret_cast<ImageHeader*>(&buffer[0]);

    header.Signature = 0x4d42; // 'BM'

    DelphiRandom random(m_randomSeed);
    for (std::size_t i = header.ImageHeight; i > 0; i--)
    {
        uint8_t* scanline = &buffer[header.BitmapOffset + (i - 1) * ((header.ImageWidth + 1) / 2)];
        for (std::size_t j = 0; j < (header.ImageWidth + 1) / 2; j++)
        {
            scanline[j] ^= random.GetNext(255);
        }
    }

    return IReadStreamPtr(new MemoryReadStream(buffer));
}

} // namespace PddBy
