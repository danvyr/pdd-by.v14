#pragma once

#include "Limb/ISectionLimb.h"

namespace Json
{

class Value;

} // namespace Json

namespace PddBy
{

class JsonSectionLimb : public ISectionLimb
{
public:
    JsonSectionLimb(Json::Value const& sectionsJson, std::size_t langId);
    virtual ~JsonSectionLimb();

    void SetLanguage(std::size_t langId);

    // ISectionLimb implementation
    virtual ISectionCrawlerPtr ListSections() const;

private:
    Json::Value const& m_sectionsJson;
    std::size_t m_langId;
};

} // namespace PddBy
