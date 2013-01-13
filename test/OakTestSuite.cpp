#include "pddby/Forest.h"
#include "pddby/Leaf/ImageLeaf.h"
#include "pddby/Leaf/QuestionLeaf.h"
#include "pddby/Leaf/SectionLeaf.h"
#include "pddby/Leaf/TopicLeaf.h"
#include "pddby/Limb/IImageLimb.h"
#include "pddby/Limb/IQuestionLimb.h"
#include "pddby/Limb/ISectionLimb.h"
#include "pddby/Limb/ITopicLimb.h"
#include "pddby/IOak.h"
#include "pddby/Shit.h"

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>

namespace
{

void DumpOak(PddBy::IOak& oak)
{
    using namespace PddBy;

    ITopicCrawlerPtr topics = oak.GetTopicLimb().ListTopics();
    TopicLeaf topicLeaf;

    std::cerr << "topics: " << std::endl;
    while (topics->GetNext(topicLeaf))
    {
        std::cerr << "  " << topicLeaf.Id << " - " << topicLeaf.Title << std::endl;
        break;
    }
    std::cerr << std::endl;

    ISectionCrawlerPtr sections = oak.GetSectionLimb().ListSections();
    SectionLeaf sectionLeaf;

    std::cerr << "sections: " << std::endl;
    while (sections->GetNext(sectionLeaf))
    {
        std::cerr << "  " << sectionLeaf.Id << " - " << sectionLeaf.Prefix << " - " << sectionLeaf.Title << std::endl;
        break;
    }
    std::cerr << std::endl;

    IQuestionCrawlerPtr questions = oak.GetQuestionLimb().ListQuestionsByRandomTicket();
    QuestionLeaf questionLeaf;

    std::cerr << "questions: " << std::endl;
    while (questions->GetNext(questionLeaf))
    {
        if (!questionLeaf.ImageId.empty())
        {
            std::cerr << "  I: " << questionLeaf.ImageId;

            ImageLeaf const imageLeaf = oak.GetImageLimb().GetImage(questionLeaf.ImageId);

            std::cerr << " (" << imageLeaf.Data.size() << " bytes)" << std::endl;
        }

        std::cerr << "  T: " << questionLeaf.TopicId << std::endl;
//        std::cerr << "  S: " << questionLeaf.SectionId << std::endl;

        std::cerr << "  Q: " << questionLeaf.Text << std::endl;

        for (std::size_t i = 0; i < questionLeaf.Answers.size(); i++)
        {
            std::cerr << "  A: " << (i == questionLeaf.CorrectAnswerIndex ? "+ " : "  ");
            std::cerr << questionLeaf.Answers[i];
            std::cerr << std::endl;
        }

        if (!questionLeaf.Advice.empty())
        {
            std::cerr << "  C: " << questionLeaf.Advice << std::endl;
        }

        std::cerr << std::endl;
        //break;
    }
}

} // anonymous namespace

BOOST_AUTO_TEST_SUITE(OakTestSuite)

BOOST_AUTO_TEST_CASE(OakTest)
{
    using namespace PddBy;

    IOakPtr oak = Forest::CreateOak("Pdd32_cd");

    if (false)
    for (boost::filesystem::directory_iterator it("Pdd32_cd/Images"), end; it != end; ++it)
    {
        std::string const imageName = it->path().filename().stem().string();

        ImageLeaf const imageLeaf = oak->GetImageLimb().GetImage(imageName);

        std::ofstream s("../.test/images/" + imageName + ".png", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&imageLeaf.Data[0]), imageLeaf.Data.size());
    }

    BOOST_REQUIRE_NO_THROW(oak->SetLanguage("ru"));
    DumpOak(*oak);
    BOOST_REQUIRE_NO_THROW(oak->SetLanguage("be"));
    DumpOak(*oak);
    BOOST_REQUIRE_THROW(oak->SetLanguage("il"), Shit);
}

BOOST_AUTO_TEST_SUITE_END()
