#pragma once

#include <string>

namespace PddBy
{

class ICommentLimb;
class IImageLimb;
class IQuestionLimb;
class ISectionLimb;
class ITopicLimb;
class ITraffRegLimb;

class IOak
{
public:
    virtual ~IOak() {}

    virtual void SetLanguage(std::string const& name) = 0;

    virtual ICommentLimb& GetCommentLimb() = 0;
    virtual IImageLimb& GetImageLimb() = 0;
    virtual IQuestionLimb& GetQuestionLimb() = 0;
    virtual ISectionLimb& GetSectionLimb() = 0;
    virtual ITopicLimb& GetTopicLimb() = 0;
    virtual ITraffRegLimb& GetTraffRegLimb() = 0;
};

} // namespace PddBy
