#include "TripleDes24Filter.h"

#include "Decode/Stream/MemoryReadStream.h"

#include <cryptopp/des.h>

#include <cstring>

namespace PddBy
{

namespace Detail
{

class Triplet
{
public:
    static std::size_t const DataSize = 3 * CryptoPP::DES::BLOCKSIZE;

public:
    Triplet(void* data) :
        m_data(static_cast<uint8_t*>(data))
    {
        //
    }

    uint8_t* GetBlock(std::size_t const i) const
    {
        return m_data + i * CryptoPP::DES::BLOCKSIZE;
    }

    void Fill(uint8_t ch)
    {
        std::memset(m_data, ch, Detail::Triplet::DataSize);
    }

    void CopyFrom(Triplet& other)
    {
        std::memcpy(m_data, other.m_data, Detail::Triplet::DataSize);
    }

    void Shuffle()
    {
        std::swap(GetWord32(1), GetWord32(2));
        // Seems like a defect: this transformation is present in original code, but is improperly coded, so it effectively
        // results in block[3] = block[3]
        // std::swap(GetWord32(3), GetWord32(4));
    }

    void Swap(Triplet& other)
    {
        std::swap(m_data, other.m_data);
    }

private:
    uint32_t& GetWord32(std::size_t const i) const
    {
        return reinterpret_cast<uint32_t*>(m_data)[i];
    }

private:
    uint8_t* m_data;
};

} // namespace Detail

TripleDes24Filter::TripleDes24Filter(IFilterPtr filter, Buffer const& key) :
    m_filter(filter),
    m_key(key)
{
    //
}

TripleDes24Filter::~TripleDes24Filter()
{
    //
}

IReadStreamPtr TripleDes24Filter::Apply(IReadStreamPtr stream)
{
    Buffer buffer;
    stream->ReadToEnd(buffer);

    CryptoPP::DES::Decryption des1(&m_key[16], 8);
    CryptoPP::DES::Encryption des2(&m_key[8], 8);
    CryptoPP::DES::Decryption des3(&m_key[0], 8);

    uint8_t temp[2][Detail::Triplet::DataSize];

    Detail::Triplet iv(&temp[0]);
    Detail::Triplet original(&temp[1]);

    iv.Fill(0xff);

    for (std::size_t i = 0; i < buffer.size() / Detail::Triplet::DataSize; i++)
    {
        Detail::Triplet triplet(&buffer[i * Detail::Triplet::DataSize]);

        original.CopyFrom(triplet);

        des1.ProcessAndXorBlock(triplet.GetBlock(0), 0, triplet.GetBlock(0));
        des1.ProcessAndXorBlock(triplet.GetBlock(1), 0, triplet.GetBlock(1));
        des1.ProcessAndXorBlock(triplet.GetBlock(2), 0, triplet.GetBlock(2));

        triplet.Shuffle();

        des2.ProcessAndXorBlock(triplet.GetBlock(0), 0, triplet.GetBlock(0));
        des2.ProcessAndXorBlock(triplet.GetBlock(1), 0, triplet.GetBlock(1));
        des2.ProcessAndXorBlock(triplet.GetBlock(2), 0, triplet.GetBlock(2));

        triplet.Shuffle();

        des3.ProcessAndXorBlock(triplet.GetBlock(0), iv.GetBlock(0), triplet.GetBlock(0));
        des3.ProcessAndXorBlock(triplet.GetBlock(1), iv.GetBlock(1), triplet.GetBlock(1));
        des3.ProcessAndXorBlock(triplet.GetBlock(2), iv.GetBlock(2), triplet.GetBlock(2));

        original.Swap(iv);
    }

    return m_filter->Apply(IReadStreamPtr(new MemoryReadStream(buffer)));
}

} // namespace PddBy
