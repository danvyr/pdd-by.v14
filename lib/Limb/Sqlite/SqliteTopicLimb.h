#pragma once

#include "Limb/IMutableTopicLimb.h"

struct sqlite3;

namespace PddBy
{

class SqliteTopicLimb : public IMutableTopicLimb
{
public:
    SqliteTopicLimb(sqlite3& sqlite);
    virtual ~SqliteTopicLimb();

    void SetLanguage(std::string const& name);

    // IMutableTopicLimb
    virtual void AddTopic(TopicLeaf const& topicLeaf);

    // ITopicLimb
    virtual ITopicCrawlerPtr ListTopics() const;

private:
    sqlite3& m_sqlite;
};

} // namespace PddBy
