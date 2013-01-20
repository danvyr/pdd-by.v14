#pragma once

#include "Limb/IMutableTraffRegLimb.h"

struct sqlite3;

namespace PddBy
{

class SqliteTraffRegLimb : public IMutableTraffRegLimb
{
public:
    SqliteTraffRegLimb(sqlite3& sqlite);
    virtual ~SqliteTraffRegLimb();

    void SetLanguage(std::string const& name);

    // IMutableTraffRegLimb
    virtual void AddTraffReg(TraffRegLeaf const& traffRegLeaf);

    // ITraffRegLimb
    virtual ITraffRegCrawlerPtr ListTraffRegs(std::string const& questionId) const;

private:
    sqlite3& m_sqlite;
};

} // namespace PddBy
