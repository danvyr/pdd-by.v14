#pragma once

#include <string>

namespace PddBy
{

struct ImageLeaf;

class IImageLimb
{
public:
    virtual ~IImageLimb() {}

    virtual ImageLeaf GetImage(std::string const& imageId) const = 0;
};

} // namespace PddBy
