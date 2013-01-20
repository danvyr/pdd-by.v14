#pragma once

#include "Limb/ITopicLimb.h"

namespace Json
{

class Value;

} // namespace Json

namespace PddBy
{

class JsonTopicLimb : public ITopicLimb
{
public:
    JsonTopicLimb(Json::Value const& topicsJson, std::size_t langId);
    virtual ~JsonTopicLimb();

    void SetLanguage(std::size_t langId);

    // ITopicLimb implementation
    virtual ITopicCrawlerPtr ListTopics() const;

private:
    Json::Value const& m_topicsJson;
    std::size_t m_langId;
};

} // namespace PddBy
