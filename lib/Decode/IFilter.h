#pragma once

#include "IReadStream.h"

#include <memory>

namespace PddBy
{

class IFilter
{
public:
    virtual ~IFilter() {}

    virtual IReadStreamPtr Apply(IReadStreamPtr stream) = 0;
};

typedef std::auto_ptr<IFilter> IFilterPtr;

} // namespace PddBy
