#pragma once

#include "Limb/ISectionLimb.h"

namespace PddBy
{

class IMutableSectionLimb : public ISectionLimb
{
public:
    virtual ~IMutableSectionLimb() {}

    virtual void AddSection(SectionLeaf const& sectionLeaf) = 0;
};

} // namespace PddBy
