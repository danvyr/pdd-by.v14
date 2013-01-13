#pragma once

#include "pddby/IOak.h"

#include "Decode/Util/Path.h"

#include <json/value.h>

#include <memory>

namespace PddBy
{

class Oak14ImageLimb;
typedef std::unique_ptr<Oak14ImageLimb> Oak14ImageLimbPtr;

class Oak14QuestionLimb;
typedef std::unique_ptr<Oak14QuestionLimb> Oak14QuestionLimbPtr;

class JsonSectionLimb;
typedef std::unique_ptr<JsonSectionLimb> JsonSectionLimbPtr;

class JsonTopicLimb;
typedef std::unique_ptr<JsonTopicLimb> JsonTopicLimbPtr;

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
    Oak14ImageLimbPtr m_imageLimb;
    Oak14QuestionLimbPtr m_questionLimb;
    JsonSectionLimbPtr m_sectionLimb;
    JsonTopicLimbPtr m_topicLimb;
};

} // namespace PddBy
