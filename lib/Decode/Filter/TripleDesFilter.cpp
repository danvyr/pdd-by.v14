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

    try
    {
        uint8_t iv[CryptoPP::DES_EDE3::BLOCKSIZE];
        std::memset(iv, 0xff, sizeof(iv));

        CryptoPP::CBC_CTS_Mode<CryptoPP::DES_EDE3>::Decryption decryptor;
        decryptor.SetKeyWithIV(&m_key[0], std::min<std::size_t>(m_key.size(), decryptor.DefaultKeyLength()), iv);
        CryptoPP::ArraySource(&buffer[0], buffer.size(), true, new CryptoPP::StreamTransformationFilter(decryptor,
            new CryptoPP::ArraySink(&buffer[0], buffer.size())));
    }
    catch (CryptoPP::Exception const& e)
    {
        throw Shit(std::string("Unable to decrypt using Triple DES: ") + e.what());
    }

    return m_filter->Apply(IReadStreamPtr(new MemoryReadStream(buffer)));
}

} // namespace PddBy
