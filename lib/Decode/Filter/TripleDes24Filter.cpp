#include "TripleDes24Filter.h"

#include "Decode/Stream/MemoryReadStream.h"

#include "pddby/Shit.h"

#include <cstring>

#include <openssl/des.h>

namespace PddBy
{

TripleDes24Filter::TripleDes24Filter(IFilterPtr filter, Buffer const& key) :
    m_filter(std::move(filter)),
    m_key(key)
{
    //
}

IReadStreamPtr TripleDes24Filter::Apply(IReadStreamPtr stream)
{
    Buffer buffer;
    stream->ReadToEnd(buffer);

    DES_key_schedule sched1;
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[16])), &sched1);

    DES_key_schedule sched2;
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[8])), &sched2);

    DES_key_schedule sched3;
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[0])), &sched3);

    std::uint32_t iv[6];
    std::memset(iv, 0xff, sizeof(iv));

    for (std::size_t i = 0; i < buffer.size() / 24; i++)
    {
        std::uint32_t* block = reinterpret_cast<std::uint32_t*>(&buffer[i * 24]);
        std::uint32_t* block1 = block;
        std::uint32_t* block2 = block + 2;
        std::uint32_t* block3 = block + 4;

        std::uint32_t original[6];
        std::memcpy(original, block, 24);

        DES_encrypt1(block1, &sched1, DES_DECRYPT);
        DES_encrypt1(block2, &sched1, DES_DECRYPT);
        DES_encrypt1(block3, &sched1, DES_DECRYPT);

        std::swap(block[1], block[2]);
        // Seems like a defect: this transformation is present in original code, but is improperly coded, so it effectively
        // results in block[3] = block[3]
        // std::swap(block[3], block[4]);

        DES_encrypt1(block1, &sched2, DES_ENCRYPT);
        DES_encrypt1(block2, &sched2, DES_ENCRYPT);
        DES_encrypt1(block3, &sched2, DES_ENCRYPT);

        std::swap(block[1], block[2]);
        // Seems like a defect: this transformation is present in original code, but is improperly coded, so it effectively
        // results in block[3] = block[3]
        // std::swap(block[3], block[4]);

        DES_encrypt1(block1, &sched3, DES_DECRYPT);
        DES_encrypt1(block2, &sched3, DES_DECRYPT);
        DES_encrypt1(block3, &sched3, DES_DECRYPT);

        for (std::size_t j = 0; j < 6; j++)
        {
            block[j] ^= iv[j];
        }

        std::memcpy(iv, original, 24);
    }

    return m_filter->Apply(IReadStreamPtr(new MemoryReadStream(std::move(buffer))));
}

} // namespace PddBy
