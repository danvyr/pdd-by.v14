#pragma once

#include "Decode/Util/Path.h"
#include "IOak.h"

#include <json/value.h>

#include <memory>

namespace PddBy
{

class Oak14ImageLimb;
class Oak14QuestionLimb;
class JsonSectionLimb;
class JsonTopicLimb;

class Oak14 : public IOak
{
public:
    Oak14(Path const& rootPath);
    virtual ~Oak14();

    // IOak implementation
    virtual void SetLanguage(std::string const& name);

    virtual ICommentLimb& GetCommentLimb();
    virtual IImageLimb& GetImageLimb();
    virtual IQuestionLimb& GetQuestionLimb();
    virtual ISectionLimb& GetSectionLimb();
    virtual ITopicLimb& GetTopicLimb();
    virtual ITraffRegLimb& GetTraffRegLimb();

private:
    Path const m_rootPath;
    Json::Value m_jsonRoot;
    std::auto_ptr<Oak14ImageLimb> m_imageLimb;
    std::auto_ptr<Oak14QuestionLimb> m_questionLimb;
    std::auto_ptr<JsonSectionLimb> m_sectionLimb;
    std::auto_ptr<JsonTopicLimb> m_topicLimb;
};

} // namespace PddBy
