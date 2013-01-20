#pragma once

#include "IMutableOak.h"

struct sqlite3;

namespace PddBy
{

class SqliteCommentLimb;
class SqliteImageLimb;
class SqliteQuestionLimb;
class SqliteSectionLimb;
class SqliteTopicLimb;
class SqliteTraffRegLimb;

class SqliteOak : public IMutableOak
{
public:
    SqliteOak(std::string const databaseName);
    virtual ~SqliteOak();

    // IMutableOak
    virtual IMutableCommentLimb& GetMutableCommentLimb();
    virtual IMutableImageLimb& GetMutableImageLimb();
    virtual IMutableQuestionLimb& GetMutableQuestionLimb();
    virtual IMutableSectionLimb& GetMutableSectionLimb();
    virtual IMutableTopicLimb& GetMutableTopicLimb();
    virtual IMutableTraffRegLimb& GetMutableTraffRegLimb();

    // IOak
    virtual void SetLanguage(std::string const& name);

    virtual ICommentLimb& GetCommentLimb();
    virtual IImageLimb& GetImageLimb();
    virtual IQuestionLimb& GetQuestionLimb();
    virtual ISectionLimb& GetSectionLimb();
    virtual ITopicLimb& GetTopicLimb();
    virtual ITraffRegLimb& GetTraffRegLimb();

private:
    sqlite3* m_sqlite;
    std::auto_ptr<SqliteCommentLimb> m_commentLimb;
    std::auto_ptr<SqliteImageLimb> m_imageLimb;
    std::auto_ptr<SqliteQuestionLimb> m_questionLimb;
    std::auto_ptr<SqliteSectionLimb> m_sectionLimb;
    std::auto_ptr<SqliteTopicLimb> m_topicLimb;
    std::auto_ptr<SqliteTraffRegLimb> m_traffRegLimb;
};

} // namespace PddBy
