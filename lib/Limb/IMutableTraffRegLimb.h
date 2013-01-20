#pragma once

#include "Limb/ITraffRegLimb.h"

namespace PddBy
{

class IMutableTraffRegLimb : public ITraffRegLimb
{
public:
    virtual ~IMutableTraffRegLimb() {}

    virtual void AddTraffReg(TraffRegLeaf const& traffRegLeaf) = 0;
};

} // namespace PddBy
