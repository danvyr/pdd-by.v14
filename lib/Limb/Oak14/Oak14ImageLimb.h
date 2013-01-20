#pragma once

#include "Decode/Util/Path.h"
#include "Limb/IImageLimb.h"

#include <stdint.h>

namespace PddBy
{

class Oak14ImageLimb : public IImageLimb
{
public:
    Oak14ImageLimb(Path const& rootPath, std::string const& magicString, uint32_t magicNumber);
    virtual ~Oak14ImageLimb();

    virtual ImageLeaf GetImage(std::string const& imageId) const;

private:
    Path const m_rootPath;
    std::string const m_magicString;
    uint32_t const m_magicNumber;
};

} // namespace PddBy
