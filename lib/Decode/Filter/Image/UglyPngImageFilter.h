#pragma once

#include "Decode/IFilter.h"

#include <string>

namespace PddBy
{

class UglyPngImageFilter : public IFilter
{
public:
    UglyPngImageFilter(std::string const& imageName);
    virtual ~UglyPngImageFilter();

    virtual IReadStreamPtr Apply(IReadStreamPtr stream);

private:
    std::string const m_imageName;
};

} // namespace PddBy
