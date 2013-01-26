#include "TripleDes24Filter.h"

#include "Decode/Stream/MemoryReadStream.h"
#include "Shit.h"

#include <cstring>

#include <openssl/des.h>

namespace PddBy
{

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

    DES_key_schedule sched[3];
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[16])), &sched[0]);
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[8])), &sched[1]);
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[0])), &sched[2]);

    uint32_t temp[2][6];

    uint32_t* iv = reinterpret_cast<uint32_t*>(&temp[0]);
    uint32_t* original = reinterpret_cast<uint32_t*>(&temp[1]);

    std::memset(iv, 0xff, sizeof(uint32_t) * 6);

    for (std::size_t i = 0; i < buffer.size() / 24; i++)
    {
        uint32_t* block = reinterpret_cast<uint32_t*>(&buffer[i * 24]);

        std::memcpy(original, block, 24);

        DES_encrypt1(&block[0], &sched[0], DES_DECRYPT);
        DES_encrypt1(&block[2], &sched[0], DES_DECRYPT);
        DES_encrypt1(&block[4], &sched[0], DES_DECRYPT);

        std::swap(block[1], block[2]);
        // Seems like a defect: this transformation is present in original code, but is improperly coded, so it effectively
        // results in block[3] = block[3]
        // std::swap(block[3], block[4]);

        DES_encrypt1(&block[0], &sched[1], DES_ENCRYPT);
        DES_encrypt1(&block[2], &sched[1], DES_ENCRYPT);
        DES_encrypt1(&block[4], &sched[1], DES_ENCRYPT);

        std::swap(block[1], block[2]);
        // Seems like a defect: this transformation is present in original code, but is improperly coded, so it effectively
        // results in block[3] = block[3]
        // std::swap(block[3], block[4]);

        DES_encrypt1(&block[0], &sched[2], DES_DECRYPT);
        DES_encrypt1(&block[2], &sched[2], DES_DECRYPT);
        DES_encrypt1(&block[4], &sched[2], DES_DECRYPT);

        *reinterpret_cast<uint64_t*>(&block[0]) ^= *reinterpret_cast<uint64_t*>(&iv[0]);
        *reinterpret_cast<uint64_t*>(&block[2]) ^= *reinterpret_cast<uint64_t*>(&iv[2]);
        *reinterpret_cast<uint64_t*>(&block[4]) ^= *reinterpret_cast<uint64_t*>(&iv[4]);

        std::swap(iv, original);
    }

    return m_filter->Apply(IReadStreamPtr(new MemoryReadStream(buffer)));
}

} // namespace PddBy
