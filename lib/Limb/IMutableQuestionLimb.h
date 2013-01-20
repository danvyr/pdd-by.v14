#pragma once

#include "Limb/IQuestionLimb.h"

namespace PddBy
{

class IMutableQuestionLimb : public IQuestionLimb
{
public:
    virtual ~IMutableQuestionLimb() {}

    virtual void AddQuestion(QuestionLeaf const& questionLeaf) = 0;
};

} // namespace PddBy
