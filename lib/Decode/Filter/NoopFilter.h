#pragma once

#include "Decode/IFilter.h"

namespace PddBy
{

class NoopFilter : public IFilter
{
public:
    NoopFilter();
    virtual ~NoopFilter();

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);
};

} // namespace PddBy
