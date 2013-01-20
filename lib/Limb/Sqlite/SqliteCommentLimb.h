#pragma once

#include "Limb/IMutableCommentLimb.h"

struct sqlite3;

namespace PddBy
{

class SqliteCommentLimb : public IMutableCommentLimb
{
public:
    SqliteCommentLimb(sqlite3& sqlite);
    virtual ~SqliteCommentLimb();

    void SetLanguage(std::string const& name);

    // IMutableCommentLimb
    virtual void AddComment(CommentLeaf const& commentLeaf);

    // ICommentLimb
    virtual CommentLeaf GetComment(std::string const& commentId) const;

private:
    sqlite3& m_sqlite;
};

} // namespace PddBy
