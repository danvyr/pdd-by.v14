#include "NoopFilter.h"

#include "Decode/IReadStream.h"

namespace PddBy
{

NoopFilter::NoopFilter()
{
    //
}

IReadStreamPtr NoopFilter::Apply(IReadStreamPtr stream)
{
    return stream;
}

} // namespace PddBy
