#pragma once

#include "Decode/IFilter.h"

#include "pddby/Buffer.h"

namespace PddBy
{

class HeaderCheckingFilter : public IFilter
{
public:
    HeaderCheckingFilter(IFilterPtr filter, Buffer const& header);

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    IFilterPtr m_filter;
    Buffer const m_header;
};

} // namespace PddBy
