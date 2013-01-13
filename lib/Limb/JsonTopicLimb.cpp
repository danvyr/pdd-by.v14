#include "JsonTopicLimb.h"

#include "pddby/Leaf/TopicLeaf.h"

#include <boost/lexical_cast.hpp>

#include <json/value.h>

namespace PddBy
{

namespace Detail
{

class JsonTopicCrawler : public ITopicCrawler
{
public:
    JsonTopicCrawler(Json::Value const& topicsJson, std::size_t langId) :
        m_iterator(topicsJson.begin()),
        m_end(topicsJson.end()),
        m_langId(langId)
    {
        //
    }

    virtual bool GetNext(TopicLeaf& value)
    {
        if (m_iterator != m_end)
        {
            Json::Value const& topicJson = *m_iterator;

            value.Id = boost::lexical_cast<std::string>(m_iterator.index());
            value.Title = topicJson[m_langId].asString();

            ++m_iterator;
            return true;
        }

        return false;
    }

private:
    Json::Value::const_iterator m_iterator;
    Json::Value::const_iterator const m_end;
    Json::ArrayIndex const m_langId;
};

} // namespace Detail

JsonTopicLimb::JsonTopicLimb(Json::Value const& topicsJson, std::size_t langId) :
    m_topicsJson(topicsJson),
    m_langId(langId)
{
    //
}

JsonTopicLimb::~JsonTopicLimb()
{
    //
}

void JsonTopicLimb::SetLanguage(std::size_t langId)
{
    m_langId = langId;
}

ITopicCrawlerPtr JsonTopicLimb::ListTopics() const
{
    return ITopicCrawlerPtr(new Detail::JsonTopicCrawler(m_topicsJson, m_langId));
}

} // namespace PddBy
