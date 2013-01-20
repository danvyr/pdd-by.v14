#pragma once

#include "Limb/IMutableQuestionLimb.h"

struct sqlite3;

namespace PddBy
{

class SqliteQuestionLimb : public IMutableQuestionLimb
{
public:
    SqliteQuestionLimb(sqlite3& sqlite);
    virtual ~SqliteQuestionLimb();

    void SetLanguage(std::string const& name);

    // IMutableQuestionLimb
    virtual void AddQuestion(QuestionLeaf const& questionLeaf);

    // IQuestionLimb
    virtual IQuestionCrawlerPtr ListQuestionsBySection(std::string const& sectionId) const;
    virtual IQuestionCrawlerPtr ListQuestionsByTopic(std::string const& topicId, std::size_t ticketNumber) const;
    virtual IQuestionCrawlerPtr ListQuestionsByTicket(std::size_t ticketNumber) const;
    virtual IQuestionCrawlerPtr ListQuestionsByRandomTicket() const;

private:
    sqlite3& m_sqlite;
};

} // namespace PddBy
