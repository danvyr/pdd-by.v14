#pragma once

#include "IOak.h"

namespace PddBy
{

class IMutableCommentLimb;
class IMutableImageLimb;
class IMutableQuestionLimb;
class IMutableSectionLimb;
class IMutableTopicLimb;
class IMutableTraffRegLimb;

class IMutableOak : public IOak
{
public:
    virtual ~IMutableOak() {}

    virtual IMutableCommentLimb& GetMutableCommentLimb() = 0;
    virtual IMutableImageLimb& GetMutableImageLimb() = 0;
    virtual IMutableQuestionLimb& GetMutableQuestionLimb() = 0;
    virtual IMutableSectionLimb& GetMutableSectionLimb() = 0;
    virtual IMutableTopicLimb& GetMutableTopicLimb() = 0;
    virtual IMutableTraffRegLimb& GetMutableTraffRegLimb() = 0;
};

} // namespace PddBy
