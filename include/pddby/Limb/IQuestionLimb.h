#pragma once

#include "../ICrawler.h"

#include <memory>
#include <string>

namespace PddBy
{

struct QuestionLeaf;
typedef ICrawler<QuestionLeaf> IQuestionCrawler;
typedef std::auto_ptr<IQuestionCrawler> IQuestionCrawlerPtr;

class IQuestionLimb
{
public:
    virtual ~IQuestionLimb() {}

    virtual IQuestionCrawlerPtr ListQuestionsBySection(std::string const& sectionId) const = 0;
    virtual IQuestionCrawlerPtr ListQuestionsByTopic(std::string const& topicId, std::size_t ticketNumber) const = 0;
    virtual IQuestionCrawlerPtr ListQuestionsByTicket(std::size_t ticketNumber) const = 0;
    virtual IQuestionCrawlerPtr ListQuestionsByRandomTicket() const = 0;
};

} // namespace PddBy
