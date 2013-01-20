#pragma once

#include "Limb/IMutableSectionLimb.h"

struct sqlite3;

namespace PddBy
{

class SqliteSectionLimb : public IMutableSectionLimb
{
public:
    SqliteSectionLimb(sqlite3& sqlite);
    virtual ~SqliteSectionLimb();

    void SetLanguage(std::string const& name);

    // IMutableSectionLimb
    virtual void AddSection(SectionLeaf const& sectionLeaf);

    // ISectionLimb
    virtual ISectionCrawlerPtr ListSections() const;

private:
    sqlite3& m_sqlite;
};

} // namespace PddBy
