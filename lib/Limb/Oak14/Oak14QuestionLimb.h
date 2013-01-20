#pragma once

#include "Limb/IQuestionLimb.h"

#include <map>
#include <vector>

namespace PddBy
{

class CodepageConverter;
class IRandomAccessReadStream;
class ISectionLimb;
class Path;

class Oak14QuestionLimb : public IQuestionLimb
{
    typedef std::vector<std::size_t> BlockList;
    typedef std::map<std::string, BlockList> IdToBlockMap;

public:
    Oak14QuestionLimb(Path const& rootPath, std::string const& magicString, ISectionLimb& sectionLimb,
        std::vector<std::size_t> const& ticketTopics, std::size_t langId);
    virtual ~Oak14QuestionLimb();

    void SetLanguage(std::size_t langId);

    // IQuestionLimb implementation
    virtual IQuestionCrawlerPtr ListQuestionsBySection(std::string const& sectionId) const;
    virtual IQuestionCrawlerPtr ListQuestionsByTopic(std::string const& topicId, std::size_t ticketNumber) const;
    virtual IQuestionCrawlerPtr ListQuestionsByTicket(std::size_t ticketNumber) const;
    virtual IQuestionCrawlerPtr ListQuestionsByRandomTicket() const;

private:
    void SetupBlockMaps(ISectionLimb& sectionLimb);

private:
    std::string const m_magicString;
    std::vector<std::size_t> const m_ticketTopics;
    std::size_t m_langId;
    std::auto_ptr<IRandomAccessReadStream> const m_stream;
    std::auto_ptr<CodepageConverter> const m_conv;
    IdToBlockMap m_sectionBlocks;
    IdToBlockMap m_topicBlocks;
};

} // namespace PddBy
