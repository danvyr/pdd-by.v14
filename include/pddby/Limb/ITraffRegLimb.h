#pragma once

#include "../ICrawler.h"

#include <memory>
#include <string>

namespace PddBy
{

struct TraffRegLeaf;
typedef ICrawler<TraffRegLeaf> ITraffRegCrawler;
typedef std::unique_ptr<ITraffRegCrawler> ITraffRegCrawlerPtr;

class ITraffRegLimb
{
public:
    virtual ~ITraffRegLimb() {}

    virtual ITraffRegCrawlerPtr ListTraffRegs(std::string const& questionId) const = 0;
};

} // namespace PddBy
