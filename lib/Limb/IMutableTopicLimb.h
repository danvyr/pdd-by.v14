#pragma once

#include "Limb/ITopicLimb.h"

namespace PddBy
{

class IMutableTopicLimb : public ITopicLimb
{
public:
    virtual ~IMutableTopicLimb() {}

    virtual void AddTopic(TopicLeaf const& topicLeaf) = 0;
};

} // namespace PddBy
