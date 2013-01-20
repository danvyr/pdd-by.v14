#pragma once

#include "../ICrawler.h"

#include <string>

namespace PddBy
{

struct CommentLeaf;

class ICommentLimb
{
public:
    virtual ~ICommentLimb() {}

    virtual CommentLeaf GetComment(std::string const& commentId) const = 0;
};

} // namespace PddBy
