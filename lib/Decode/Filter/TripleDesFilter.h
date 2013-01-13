#pragma once

#include "Decode/IFilter.h"

#include "pddby/Buffer.h"

namespace PddBy
{

class TripleDesFilter : public IFilter
{
public:
    TripleDesFilter(IFilterPtr filter, Buffer const& key);

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    IFilterPtr m_filter;
    Buffer const m_key;
};

} // namespace PddBy
