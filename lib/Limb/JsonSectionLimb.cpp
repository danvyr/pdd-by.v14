#include "JsonSectionLimb.h"

#include "pddby/Leaf/SectionLeaf.h"

#include <json/value.h>

namespace PddBy
{

namespace Detail
{

class JsonSectionCrawler : public ISectionCrawler
{
public:
    JsonSectionCrawler(Json::Value const& sectionsJson, std::size_t langId) :
        m_iterator(sectionsJson.begin()),
        m_end(sectionsJson.end()),
        m_langId(1 + langId)
    {
        //
    }

    virtual bool GetNext(SectionLeaf& value)
    {
        if (m_iterator != m_end)
        {
            Json::Value const& sectionJson = *m_iterator;

            value.Id = sectionJson[0].asString();
            value.Prefix = sectionJson[m_langId][0].asString();
            value.Title = sectionJson[m_langId][1].asString();

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

JsonSectionLimb::JsonSectionLimb(Json::Value const& sectionsJson, std::size_t langId) :
    m_sectionsJson(sectionsJson),
    m_langId(langId)
{
    //
}

JsonSectionLimb::~JsonSectionLimb()
{
    //
}

void JsonSectionLimb::SetLanguage(std::size_t langId)
{
    m_langId = langId;
}

ISectionCrawlerPtr JsonSectionLimb::ListSections() const
{
    return ISectionCrawlerPtr(new Detail::JsonSectionCrawler(m_sectionsJson, m_langId));
}

} // namespace PddBy
