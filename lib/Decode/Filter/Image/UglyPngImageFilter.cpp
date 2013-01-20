#include "UglyPngImageFilter.h"

#include "BpftHelper.h"

#include "Decode/Stream/MemoryReadStream.h"
#include "Decode/Stream/MemoryWriteStream.h"
#include "Decode/Stream/StreamShit.h"

#include <boost/format.hpp>

#include <cryptopp/crc.h>

#include <algorithm>

namespace
{

PddBy::Buffer BufferFromString(std::string const& s)
{
    PddBy::Buffer buffer;
    for (std::size_t i = 0; i < s.size(); i++)
    {
        buffer.push_back(static_cast<PddBy::Buffer::value_type>(s[i]));
    }

    return buffer;
}

} // anonymous namespace

namespace PddBy
{

namespace Magic
{

Buffer const PngFileHeader = BufferFromString("\x89PNG\r\n\x1a\n");

uint16_t const InitialImageNameSeed = 0x1a80;

namespace PngChunk
{

#define PNG_CHUNK_TYPE(x) #x[0] | (#x[1] << 8) | (#x[2] << 16) | (#x[3] << 24)

uint32_t const IHDR = PNG_CHUNK_TYPE(IHDR);
uint32_t const IDAT = PNG_CHUNK_TYPE(IDAT);

#undef PNG_CHUNK_TYPE

} // namespace PngChunk

} // namespace Magic

UglyPngImageFilter::UglyPngImageFilter(std::string const& imageName) :
    m_imageName(imageName)
{
    //
}

UglyPngImageFilter::~UglyPngImageFilter()
{
    //
}

IReadStreamPtr UglyPngImageFilter::Apply(IReadStreamPtr stream)
{
    // TODO: Big-endian support

    Buffer outputBuffer;
    MemoryWriteStream outputStream(outputBuffer);

    outputStream.Write(Magic::PngFileHeader);

    uint16_t magic1[2];
    stream->Read(&magic1, sizeof(magic1));

    uint16_t const magic2 = BpftHelper::ImageNameToRandSeed(m_imageName, 0 ^ Magic::InitialImageNameSeed);
    if ((magic1[0] ^ magic1[1]) != magic2)
    {
        throw Shit(boost::format("Image magic doesn't match: 0x%04x != 0x%04x") % magic1 % magic2);
    }

    for (;;)
    {
        uint32_t chunkSize = 0;

        try
        {
            stream->Read(&chunkSize, 1);
        }
        catch (EndOfStreamShit const&)
        {
            break;
        }

        stream->Read(reinterpret_cast<uint8_t*>(&chunkSize) + 1, sizeof(chunkSize) - 1);

        chunkSize ^= 0xff;

        uint32_t chunkType = 0;
        stream->Read(&chunkType, sizeof(chunkType));
        if (chunkType == 'p')
        {
            chunkType = Magic::PngChunk::IHDR;
        }
        else if (chunkType == 'l')
        {
            chunkType = Magic::PngChunk::IDAT;
        }

        Buffer chunkData;
        stream->Read(chunkData, chunkSize);

        if (chunkType == Magic::PngChunk::IHDR)
        {
            // Last chunk byte is (intentionally?) not XORed
            for (std::size_t i = 1; i < chunkData.size(); i++)
            {
                chunkData[i - 1] ^= 13 * (i + 1);
            }

            std::reverse(chunkData.begin(), chunkData.begin() + 4);
            std::reverse(chunkData.begin() + 4, chunkData.begin() + 8);
        }
        else
        {
            for (std::size_t i = 0; i < (chunkData.size() + 0xffff - 1) / 0xffff; i++)
            {
                std::size_t const jMax = std::min<std::size_t>(0xffff, chunkData.size() - i * 0xffff);

                // First byte of each 64K chunk block is not XORed (someone mixed up C and Pascal array indexing?)
                for (std::size_t j = 1; j < jMax; j++)
                {
                    chunkData[i * 0xffff + j] ^= j % 0xff;
                }
            }
        }

        CryptoPP::CRC32 crcHasher;
        crcHasher.Update(reinterpret_cast<uint8_t const*>(&chunkType), sizeof(chunkType));
        crcHasher.Update(&chunkData[0], chunkData.size());

        uint8_t chunkCrc[CryptoPP::CRC32::DIGESTSIZE];
        crcHasher.Final(chunkCrc);

        std::reverse(reinterpret_cast<uint8_t*>(&chunkSize), reinterpret_cast<uint8_t*>(&chunkSize) +
            sizeof(chunkSize));
        std::reverse(chunkCrc, chunkCrc + sizeof(chunkCrc));

        outputStream.Write(&chunkSize, sizeof(chunkSize));
        outputStream.Write(&chunkType, sizeof(chunkType));
        outputStream.Write(chunkData);
        outputStream.Write(&chunkCrc, sizeof(chunkCrc));
    }

    return IReadStreamPtr(new MemoryReadStream(outputBuffer));
}

} // namespace PddBy
