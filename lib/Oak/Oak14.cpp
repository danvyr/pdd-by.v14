#include "Oak14.h"

#include "Oak14/Oak14ImageLimb.h"
#include "Oak14/Oak14QuestionLimb.h"

#include "Decode/Util/Hash.h"
#include "Limb/JsonSectionLimb.h"
#include "Limb/JsonTopicLimb.h"

#include "pddby/Shit.h"

#include <boost/algorithm/string/predicate.hpp>

#include <json/reader.h>

#include <fstream>

namespace PddBy
{

namespace Detail
{

void GetMagicFromJson(Json::Value const& jsonMagic, Path const& pdd32ExePath, std::uint32_t& magicNumber,
    std::string& magicString)
{
    std::string const checksum = Hash::ToHexString(Hash::Md5FromFile(pdd32ExePath));

    for (Json::Value::const_iterator it = jsonMagic.begin(), end = jsonMagic.end(); it != end; ++it)
    {
        std::string const currentChecksum = it.key().asString();
        if (!boost::algorithm::iequals(currentChecksum, checksum))
        {
            continue;
        }

        magicNumber = (*it)[0].asUInt();
        magicString = (*it)[1].asString();
        return;
    }

    throw Shit("This version is not supported");
}

} // namespace Detail

Oak14::Oak14(Path const& rootPath) :
    m_rootPath(rootPath)
{
    std::ifstream jsonStream("data/oak14.json");
    Json::Reader jsonReader;
    if (!jsonReader.parse(jsonStream, m_jsonRoot))
    {
        throw Shit("Unable to parse JSON data: " + jsonReader.getFormattedErrorMessages());
    }

    std::string magicString;
    std::uint32_t magicNumber;
    Detail::GetMagicFromJson(m_jsonRoot["magic"], m_rootPath / "pdd32.exe", magicNumber, magicString);

    m_sectionLimb.reset(new JsonSectionLimb(m_jsonRoot["sections"], 0));
    m_topicLimb.reset(new JsonTopicLimb(m_jsonRoot["topics"], 0));

    m_imageLimb.reset(new Oak14ImageLimb(m_rootPath, magicString, magicNumber));

    std::vector<std::size_t> ticketTopics;
    Json::Value jsonTicketTopics = m_jsonRoot["ticket_topics"];
    for (Json::ArrayIndex i = 0; i < jsonTicketTopics.size(); i++)
    {
        ticketTopics.push_back(jsonTicketTopics[i].asInt());
    }

    m_questionLimb.reset(new Oak14QuestionLimb(m_rootPath, magicString, *m_sectionLimb, ticketTopics, 0));
}

Oak14::~Oak14()
{
    //
}

void Oak14::SetLanguage(std::string const& name)
{
    std::size_t langId = static_cast<std::size_t>(-1);
    for (Json::ArrayIndex i = 0; i < m_jsonRoot["lang"].size(); i++)
    {
        if (m_jsonRoot["lang"][i].asString() == name)
        {
            langId = i;
            break;
        }
    }

    if (langId == static_cast<std::size_t>(-1))
    {
        throw Shit("Language \"" + name + "\" is not supported");
    }

    m_questionLimb->SetLanguage(langId);
    m_sectionLimb->SetLanguage(langId);
    m_topicLimb->SetLanguage(langId);
}

ICommentLimb& Oak14::GetCommentLimb()
{
    throw Shit("Not implemented");
}

IImageLimb& Oak14::GetImageLimb()
{
    return *m_imageLimb;
}

IQuestionLimb& Oak14::GetQuestionLimb()
{
    return *m_questionLimb;
}

ISectionLimb& Oak14::GetSectionLimb()
{
    return *m_sectionLimb;
}

ITopicLimb& Oak14::GetTopicLimb()
{
    return *m_topicLimb;
}

ITraffRegLimb& Oak14::GetTraffRegLimb()
{
    throw Shit("Not implemented");
}

} // namespace PddBy
