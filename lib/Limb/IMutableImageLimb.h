#pragma once

#include "Limb/IImageLimb.h"

namespace PddBy
{

class IMutableImageLimb : public IImageLimb
{
public:
    virtual ~IMutableImageLimb() {}

    virtual void AddImage(ImageLeaf const& imageLeaf) = 0;
};

} // namespace PddBy
