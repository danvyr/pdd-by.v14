#include "TripleDesFilter.h"

#include "Decode/Stream/MemoryReadStream.h"
#include "Shit.h"

#include <cryptopp/des.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>

#include <cstring>

namespace PddBy
{

TripleDesFilter::TripleDesFilter(IFilterPtr filter, Buffer const& key) :
    m_filter(filter),
    m_key(key)
{
    //
}

TripleDesFilter::~TripleDesFilter()
{
    //
}

IReadStreamPtr TripleDesFilter::Apply(IReadStreamPtr stream)
{
    Buffer buffer;
    stream->ReadToEnd(buffer);

    buffer.reserve((buffer.size() + 7) & ~7);

    uint8_t iv[CryptoPP::DES_EDE3::BLOCKSIZE];
    std::memset(iv, 0xff, sizeof(iv));

    uint8_t tail[CryptoPP::DES_EDE3::BLOCKSIZE];
    std::memcpy(tail, &buffer[(buffer.size() - 8) & ~7], sizeof(tail));

    CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Decryption des1(&m_key[0], 24, iv);
    CryptoPP::ArraySource(&buffer[0], buffer.size() & ~7, true, new CryptoPP::StreamTransformationFilter(des1,
        new CryptoPP::ArraySink(&buffer[0], buffer.size() & ~7), CryptoPP::StreamTransformationFilter::NO_PADDING));

    if ((buffer.size() & 7) != 0)
    {
        CryptoPP::DES_EDE3::Encryption des2(&m_key[0], 24);
        des2.ProcessAndXorBlock(tail, &buffer[buffer.size() & ~7], &buffer[buffer.size() & ~7]);
    }

    return m_filter->Apply(IReadStreamPtr(new MemoryReadStream(buffer)));
}

} // namespace PddBy
