#pragma once

#include "Limb/ICommentLimb.h"

namespace PddBy
{

class IMutableCommentLimb : public ICommentLimb
{
public:
    virtual ~IMutableCommentLimb() {}

    virtual void AddComment(CommentLeaf const& commentLeaf) = 0;
};

} // namespace PddBy
