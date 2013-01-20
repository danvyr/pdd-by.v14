#pragma once

#include "../ICrawler.h"

#include <memory>

namespace PddBy
{

struct SectionLeaf;
typedef ICrawler<SectionLeaf> ISectionCrawler;
typedef std::auto_ptr<ISectionCrawler> ISectionCrawlerPtr;

class ISectionLimb
{
public:
    virtual ~ISectionLimb() {}

    virtual ISectionCrawlerPtr ListSections() const = 0;
};

} // namespace PddBy
