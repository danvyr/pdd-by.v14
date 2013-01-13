#pragma once

#include <memory>

namespace PddBy
{

class IReadStream;
typedef std::unique_ptr<IReadStream> IReadStreamPtr;

class IFilter
{
public:
    virtual ~IFilter() {}

    virtual IReadStreamPtr Apply(IReadStreamPtr stream) = 0;
};

typedef std::unique_ptr<IFilter> IFilterPtr;

} // namespace PddBy
