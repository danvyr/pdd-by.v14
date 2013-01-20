#pragma once

#include "Decode/IFilter.h"
#include "Buffer.h"

namespace PddBy
{

class TripleDes24Filter : public IFilter
{
public:
    TripleDes24Filter(IFilterPtr filter, Buffer const& key);
    virtual ~TripleDes24Filter();

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    IFilterPtr m_filter;
    Buffer const m_key;
};

} // namespace PddBy
