#include "HeaderCheckingFilter.h"

#include "Decode/IReadStream.h"
#include "Decode/Util/Hash.h"

#include "pddby/Shit.h"

namespace PddBy
{

HeaderCheckingFilter::HeaderCheckingFilter(IFilterPtr filter, Buffer const& header) :
    m_filter(std::move(filter)),
    m_header(header)
{
    //
}

IReadStreamPtr HeaderCheckingFilter::Apply(IReadStreamPtr stream)
{
    Buffer tempBuffer;
    stream->Read(tempBuffer, m_header.size());
    if (tempBuffer != m_header)
    {
        throw Shit("Stream header doesn't match: " + Hash::ToHexString(tempBuffer) + " vs. " + Hash::ToHexString(m_header));
    }

    return m_filter->Apply(std::move(stream));
}

} // namespace PddBy
