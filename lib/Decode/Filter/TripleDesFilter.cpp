#include "TripleDesFilter.h"

#include "Decode/Stream/MemoryReadStream.h"
#include "Shit.h"

#include <cstring>

#include <openssl/des.h>

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

    DES_key_schedule sched[3];
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[0])), &sched[0]);
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[8])), &sched[1]);
    DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[16])), &sched[2]);

    DES_cblock iv;
    std::memset(&iv, 0xff, sizeof(iv));

    DES_ede3_cbc_encrypt(&buffer[0], &buffer[0], buffer.size() & ~7, &sched[0], &sched[1], &sched[2], &iv, DES_DECRYPT);

    if ((buffer.size() & 7) != 0)
    {
        buffer.reserve((buffer.size() + 7) & ~7);

        DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[0])), &sched[0]);
        DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[8])), &sched[1]);
        DES_set_key(const_cast<DES_cblock*>(reinterpret_cast<DES_cblock const*>(&m_key[16])), &sched[2]);

        DES_ecb3_encrypt(&iv, &iv, &sched[0], &sched[1], &sched[2], DES_ENCRYPT);

        *reinterpret_cast<uint64_t*>(&buffer[buffer.size() & ~7]) ^= *reinterpret_cast<uint64_t*>(&iv);
    }

    return m_filter->Apply(IReadStreamPtr(new MemoryReadStream(buffer)));
}

} // namespace PddBy
