#include "SqliteOak.h"

#include "Limb/Sqlite/SqliteCommentLimb.h"
#include "Limb/Sqlite/SqliteImageLimb.h"
#include "Limb/Sqlite/SqliteQuestionLimb.h"
#include "Limb/Sqlite/SqliteSectionLimb.h"
#include "Limb/Sqlite/SqliteTopicLimb.h"
#include "Limb/Sqlite/SqliteTraffRegLimb.h"
#include "Shit.h"

#include <sqlite3.h>

namespace PddBy
{

SqliteOak::SqliteOak(std::string const databaseName) :
    m_sqlite(0)
{
    int result = sqlite3_open(databaseName.c_str(), &m_sqlite);
    if (result != SQLITE_OK)
    {
        throw Shit(std::string("Unable to open database: ") + sqlite3_errmsg(m_sqlite));
    }

    m_commentLimb.reset(new SqliteCommentLimb(*m_sqlite));
    m_imageLimb.reset(new SqliteImageLimb(*m_sqlite));
    m_questionLimb.reset(new SqliteQuestionLimb(*m_sqlite));
    m_sectionLimb.reset(new SqliteSectionLimb(*m_sqlite));
    m_topicLimb.reset(new SqliteTopicLimb(*m_sqlite));
    m_traffRegLimb.reset(new SqliteTraffRegLimb(*m_sqlite));
}

SqliteOak::~SqliteOak()
{
    if (m_sqlite != 0)
    {
        sqlite3_close(m_sqlite);
    }
}

IMutableCommentLimb& SqliteOak::GetMutableCommentLimb()
{
    return *m_commentLimb;
}

IMutableImageLimb& SqliteOak::GetMutableImageLimb()
{
    return *m_imageLimb;
}

IMutableQuestionLimb& SqliteOak::GetMutableQuestionLimb()
{
    return *m_questionLimb;
}

IMutableSectionLimb& SqliteOak::GetMutableSectionLimb()
{
    return *m_sectionLimb;
}

IMutableTopicLimb& SqliteOak::GetMutableTopicLimb()
{
    return *m_topicLimb;
}

IMutableTraffRegLimb& SqliteOak::GetMutableTraffRegLimb()
{
    return *m_traffRegLimb;
}

void SqliteOak::SetLanguage(std::string const& name)
{
    m_commentLimb->SetLanguage(name);
    m_questionLimb->SetLanguage(name);
    m_sectionLimb->SetLanguage(name);
    m_topicLimb->SetLanguage(name);
    m_traffRegLimb->SetLanguage(name);
}

ICommentLimb& SqliteOak::GetCommentLimb()
{
    return GetMutableCommentLimb();
}

IImageLimb& SqliteOak::GetImageLimb()
{
    return GetMutableImageLimb();
}

IQuestionLimb& SqliteOak::GetQuestionLimb()
{
    return GetMutableQuestionLimb();
}

ISectionLimb& SqliteOak::GetSectionLimb()
{
    return GetMutableSectionLimb();
}

ITopicLimb& SqliteOak::GetTopicLimb()
{
    return GetMutableTopicLimb();
}

ITraffRegLimb& SqliteOak::GetTraffRegLimb()
{
    return GetMutableTraffRegLimb();
}

} //namespace PddBy
