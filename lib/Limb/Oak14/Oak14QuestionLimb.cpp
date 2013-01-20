#include "Oak14QuestionLimb.h"

#include "Decode/Filter/NoopFilter.h"
#include "Decode/Filter/TripleDes24Filter.h"
#include "Decode/Random/DelphiRandom.h"
#include "Decode/Stream/FileReadStream.h"
#include "Decode/Stream/MemoryReadStream.h"
#include "Decode/Stream/StreamShit.h"
#include "Decode/Util/CodepageConverter.h"
#include "Decode/Util/Hash.h"
#include "Decode/Util/Path.h"
#include "Leaf/QuestionLeaf.h"
#include "Leaf/SectionLeaf.h"
#include "Limb/ISectionLimb.h"
#include "Shit.h"

#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>

#include <cassert>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace PddBy
{

namespace Detail
{

struct __attribute__((packed)) ImageStringInfo
{
    uint8_t Length;
    char Text[32];
};

struct __attribute__((packed)) QuestionStringInfo
{
    uint16_t Length;
    char Text[300];
};

struct __attribute__((packed)) AnswerStringInfo
{
    uint16_t Length;
    char Text[400];
};

struct __attribute__((packed)) AdviceStringInfo
{
    uint8_t Length;
    char Text[64];
};

struct __attribute__((packed)) BlockDataInfo
{
    uint8_t Pad1[0x10];
    uint8_t ZeroIfValidBlock;
    uint8_t SectionBits[6];
    uint8_t TopicNumber;
    ImageStringInfo Image;
    uint8_t Pad2[1];
    QuestionStringInfo Questions[2];
    uint8_t Xxx;
    uint8_t AnswerCount;
    AnswerStringInfo Answers[2][5];
    uint8_t AnswerIndex;
    AdviceStringInfo Advices[2];
    uint8_t Pad3[1];
    uint16_t Xxx2[0x10];
    uint16_t Xxx3;
    uint8_t Pad4[0x20];
};

struct __attribute__((packed)) BlockInfo
{
    uint8_t Pad1[0x10];
    uint16_t Magic1;
    uint16_t Magic2;
    uint8_t Pad2[0x10];
    BlockDataInfo Data;
    uint8_t Pad3[0x10];
};

BOOST_STATIC_ASSERT(sizeof(BlockInfo) == 4934);

template<typename StringInfoT>
std::string StringInfoToString(StringInfoT const& stringInfo)
{
    return std::string(stringInfo.Text, stringInfo.Length);
}

BlockDataInfo ReadBlockData(IRandomAccessReadStream& stream, std::size_t index, std::string const& magicString)
{
    BlockInfo blockInfo;

    stream.SetPosition(index * sizeof(blockInfo));
    stream.Read(&blockInfo, sizeof(blockInfo));

    std::ostringstream blockMagicStream;
    blockMagicStream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << blockInfo.Magic2;
    blockMagicStream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << blockInfo.Magic1;

    IFilterPtr filter(new TripleDes24Filter(IFilterPtr(new NoopFilter()), Hash::RipeMd256FromString(blockMagicStream.str() +
        magicString)));

    Buffer blockData(sizeof(blockInfo.Data));
    std::memcpy(&blockData[0], &blockInfo.Data, sizeof(blockInfo.Data));

    IReadStreamPtr encodedBlockStream(new MemoryReadStream(blockData));
    IReadStreamPtr decodedBlockStream = filter->Apply(encodedBlockStream);

    decodedBlockStream->Read(&blockInfo.Data, sizeof(blockInfo.Data));

    if (blockInfo.Data.ZeroIfValidBlock != 0)
    {
        throw Shit("Unable to decode question block");
    }

    return blockInfo.Data;
}

QuestionLeaf BlockDataInfoToQuestionLeaf(BlockDataInfo const& blockDataInfo, std::size_t langId, CodepageConverter& conv)
{
    QuestionLeaf questionLeaf;

    //questionLeaf.Id = boost::lexical_cast<std::string>(index + 1);

    unsigned int topicNumber = std::min<unsigned int>(blockDataInfo.TopicNumber, 7);
    questionLeaf.TopicId = boost::lexical_cast<std::string>(topicNumber);

    //for (std::size_t i = 0; i < sizeof(blockDataInfo.SectionBits) * 8; i++)
    //{
    //    //char buffer[3];
    //    //std::sprintf(buffer, "%02x", blockDataInfo.SectionBits[i]);
    //    //questionLeaf.SectionId += buffer;
    //    questionLeaf.SectionId += blockDataInfo.SectionBits[i / 8] & (1 << (i % 8)) ? '#' : '-';
    //}

    if (blockDataInfo.Image.Length > 0)
    {
        if (!(
            blockDataInfo.Image.Length == 5 &&
            blockDataInfo.Image.Text[0] - 0x30u < 10u && // '0'..'9'
            blockDataInfo.Image.Text[1] == '_' &&
            blockDataInfo.Image.Text[2] - 0x30u < 10u && // '0'..'9'
            blockDataInfo.Image.Text[3] - 0x30u < 10u && // '0'..'9'
            blockDataInfo.Image.Text[4] - 0x30u < 10u    // '0'..'9'
            ))
        {
            throw Shit("Unsupported image name format");
        }

        // Hand-made std::atoi() with first two digits swapped
        uint32_t const randSeed =
            (blockDataInfo.Image.Text[2] - 0x30u) * 1000 +
            (blockDataInfo.Image.Text[0] - 0x30u) * 100 +
            (blockDataInfo.Image.Text[3] - 0x30u) * 10 +
            (blockDataInfo.Image.Text[4] - 0x30u);

        DelphiRandom random(randSeed);
        std::string const realImageName = boost::lexical_cast<std::string>(random.GetNext(0x1FFFFFF) + 0x1FFFFFF);

        questionLeaf.ImageId = "images/" + realImageName;
    }

    //questionLeaf.CommentId;

    questionLeaf.Text = conv.Convert(StringInfoToString(blockDataInfo.Questions[langId]));

    for (std::size_t i = 0; i < blockDataInfo.AnswerCount; i++)
    {
        questionLeaf.Answers.push_back(conv.Convert(StringInfoToString(blockDataInfo.Answers[langId][i])));
    }

    questionLeaf.CorrectAnswerIndex = blockDataInfo.AnswerIndex - 1;
    questionLeaf.Advice = conv.Convert(StringInfoToString(blockDataInfo.Advices[langId]));

    //std::cerr << "#: " << blockDataInfo.Xxx2 << " " << blockDataInfo.Xxx3 << " " << blockDataInfo.Xxx4 << std::endl;

    return questionLeaf;
}

class TestQuestionCrawler : public IQuestionCrawler
{
public:
    TestQuestionCrawler(std::string const& magicString, IRandomAccessReadStream& stream, CodepageConverter& conv,
        std::size_t langId) :
        m_magicString(magicString),
        m_stream(stream),
        m_conv(conv),
        m_langId(langId),
        m_pos(0)
    {
        //
    }

    virtual bool GetNext(QuestionLeaf& value)
    {
        try
        {
            BlockDataInfo const blockDataInfo = ReadBlockData(m_stream, m_pos++, m_magicString);
            value = BlockDataInfoToQuestionLeaf(blockDataInfo, m_langId, m_conv);
            return true;
        }
        catch (EndOfStreamShit const&)
        {
            return false;
        }
    }

private:
    std::string const m_magicString;
    IRandomAccessReadStream& m_stream;
    CodepageConverter& m_conv;
    std::size_t const m_langId;
    std::size_t m_pos;
};

class QuestionCrawler : public IQuestionCrawler
{
public:
    QuestionCrawler(std::vector<std::size_t> const& blockNumbers, IRandomAccessReadStream& stream,
        std::string const& magicString, std::size_t langId, CodepageConverter& conv) :
        m_blockNumbers(blockNumbers),
        m_stream(stream),
        m_magicString(magicString),
        m_langId(langId),
        m_conv(conv),
        m_pos(0)
    {
        //
    }

    virtual bool GetNext(QuestionLeaf& value)
    {
        if (m_pos < m_blockNumbers.size())
        {
            BlockDataInfo const blockDataInfo = ReadBlockData(m_stream, m_blockNumbers[m_pos++], m_magicString);
            value = BlockDataInfoToQuestionLeaf(blockDataInfo, m_langId, m_conv);
            return true;
        }

        return false;
    }

private:
    std::vector<std::size_t> const m_blockNumbers;
    IRandomAccessReadStream& m_stream;
    std::string const m_magicString;
    std::size_t const m_langId;
    CodepageConverter& m_conv;
    std::size_t m_pos;
};

} // namespace Detail

Oak14QuestionLimb::Oak14QuestionLimb(Path const& rootPath, std::string const& magicString, ISectionLimb& sectionLimb,
    std::vector<std::size_t> const& ticketTopics, std::size_t langId) :
    m_magicString(magicString),
    m_ticketTopics(ticketTopics),
    m_langId(langId),
    m_stream(new FileReadStream(rootPath / "tickets" / "tickets.dbt")),
    m_conv(new CodepageConverter("utf8", "cp1251"))
{
    SetupBlockMaps(sectionLimb);
}

Oak14QuestionLimb::~Oak14QuestionLimb()
{
    //
}

void Oak14QuestionLimb::SetLanguage(std::size_t langId)
{
    m_langId = langId;
}

IQuestionCrawlerPtr Oak14QuestionLimb::ListQuestionsBySection(std::string const& sectionId) const
{
    IdToBlockMap::const_iterator sectionBlocksIt = m_sectionBlocks.find(sectionId);
    if (sectionBlocksIt == m_sectionBlocks.end())
    {
        throw Shit("Section \"" + sectionId + "\" is not valid");
    }

    BlockList const& sectionBlocks = sectionBlocksIt->second;

    return IQuestionCrawlerPtr(new Detail::QuestionCrawler(sectionBlocks, *m_stream, m_magicString, m_langId, *m_conv));
}

IQuestionCrawlerPtr Oak14QuestionLimb::ListQuestionsByTopic(std::string const& topicId, std::size_t ticketNumber) const
{
    IdToBlockMap::const_iterator topicBlocksIt = m_topicBlocks.find(topicId);
    if (topicBlocksIt == m_topicBlocks.end())
    {
        throw Shit("Topic \"" + topicId + "\" is not valid");
    }

    BlockList const& topicBlocks = topicBlocksIt->second;

    BlockList blocks;
    for (std::size_t i = ticketNumber * 10; i < (ticketNumber + 1) * 10; i++)
    {
        blocks.push_back(topicBlocks[i % topicBlocks.size()]);
    }

    return IQuestionCrawlerPtr(new Detail::QuestionCrawler(blocks, *m_stream, m_magicString, m_langId, *m_conv));
}

IQuestionCrawlerPtr Oak14QuestionLimb::ListQuestionsByTicket(std::size_t ticketNumber) const
{
    BlockList blocks;
    std::map<std::string, std::size_t> topicQuestionsCount;
    for (std::size_t i = 0; i < m_ticketTopics.size(); i++)
    {
        std::string const topicId = boost::lexical_cast<std::string>(m_ticketTopics[i]);
        IdToBlockMap::const_iterator topicBlocksIt = m_topicBlocks.find(topicId);
        if (topicBlocksIt == m_topicBlocks.end())
        {
            throw Shit("Topic \"" + topicId + "\" is not valid");
        }

        BlockList const& topicBlocks = topicBlocksIt->second;
        blocks.push_back(topicBlocks[(ticketNumber * 10 + topicQuestionsCount[topicId]++) % topicBlocks.size()]);
    }

    return IQuestionCrawlerPtr(new Detail::QuestionCrawler(blocks, *m_stream, m_magicString, m_langId, *m_conv));
}

IQuestionCrawlerPtr Oak14QuestionLimb::ListQuestionsByRandomTicket() const
{
    BlockList blocks;
    for (std::size_t i = 0; i < m_ticketTopics.size(); i++)
    {
        std::string const topicId = boost::lexical_cast<std::string>(m_ticketTopics[i]);
        IdToBlockMap::const_iterator topicBlocksIt = m_topicBlocks.find(topicId);
        if (topicBlocksIt == m_topicBlocks.end())
        {
            throw Shit("Topic \"" + topicId + "\" is not valid");
        }

        BlockList const& topicBlocks = topicBlocksIt->second;
        blocks.push_back(topicBlocks[std::rand() % topicBlocks.size()]);
    }

    return IQuestionCrawlerPtr(new Detail::QuestionCrawler(blocks, *m_stream, m_magicString, m_langId, *m_conv));
}

void Oak14QuestionLimb::SetupBlockMaps(ISectionLimb& sectionLimb)
{
    m_sectionBlocks.clear();
    m_topicBlocks.clear();

    std::vector<std::string> sectionIds;

    {
        ISectionCrawlerPtr sections = sectionLimb.ListSections();
        SectionLeaf sectionLeaf;
        while (sections->GetNext(sectionLeaf))
        {
            sectionIds.push_back(sectionLeaf.Id);
        }
    }

    for (std::size_t i = 0; ; i++)
    {
        try
        {
            Detail::BlockDataInfo const blockDataInfo = Detail::ReadBlockData(*m_stream, i, m_magicString);

            for (std::size_t j = 0; j < sizeof(blockDataInfo.SectionBits) * 8; j++)
            {
                if (blockDataInfo.SectionBits[j / 8] & (1 << (j % 8)))
                {
                    m_sectionBlocks[sectionIds.at(j)].push_back(i);
                }
            }

            unsigned int topicNumber = std::min<unsigned int>(blockDataInfo.TopicNumber, 7);
            std::string const topicId = boost::lexical_cast<std::string>(topicNumber);
            m_topicBlocks[topicId].push_back(i);
        }
        catch (EndOfStreamShit const&)
        {
            break;
        }
    }
}

} // namespace PddBy
