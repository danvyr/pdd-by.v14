#pragma once

#include "pddby/Limb/IQuestionLimb.h"

#include <map>
#include <vector>

namespace PddBy
{

class ISectionLimb;
class Path;

class IRandomAccessReadStream;
typedef std::unique_ptr<IRandomAccessReadStream> IRandomAccessReadStreamPtr;

class CodepageConverter;
typedef std::unique_ptr<CodepageConverter> CodepageConverterPtr;

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
    IRandomAccessReadStreamPtr m_stream;
    CodepageConverterPtr m_conv;
    IdToBlockMap m_sectionBlocks;
    IdToBlockMap m_topicBlocks;
};

} // namespace PddBy
