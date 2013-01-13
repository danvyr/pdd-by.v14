#pragma once

#include "../ICrawler.h"

#include <memory>

namespace PddBy
{

struct TopicLeaf;
typedef ICrawler<TopicLeaf> ITopicCrawler;
typedef std::unique_ptr<ITopicCrawler> ITopicCrawlerPtr;

class ITopicLimb
{
public:
    virtual ~ITopicLimb() {}

    virtual ITopicCrawlerPtr ListTopics() const = 0;
};

} // namespace PddBy
