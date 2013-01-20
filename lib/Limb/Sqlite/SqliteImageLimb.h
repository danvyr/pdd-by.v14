#pragma once

#include "Limb/IMutableImageLimb.h"

struct sqlite3;

namespace PddBy
{

class SqliteImageLimb : public IMutableImageLimb
{
public:
    SqliteImageLimb(sqlite3& sqlite);
    virtual ~SqliteImageLimb();

    // IMutableImageLimb
    virtual void AddImage(ImageLeaf const& imageLeaf);

    // IImageLimb
    virtual ImageLeaf GetImage(std::string const& imageId) const;

private:
    sqlite3& m_sqlite;
};

} // namespace PddBy
