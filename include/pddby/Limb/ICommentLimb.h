#pragma once

#include "../ICrawler.h"

#include <memory>
#include <string>

namespace PddBy
{

struct CommentLeaf;
typedef ICrawler<CommentLeaf> ICommentCrawler;
typedef std::unique_ptr<ICommentCrawler> ICommentCrawlerPtr;

class ICommentLimb
{
public:
    virtual ~ICommentLimb() {}

    virtual CommentLeaf GetComment(std::string const& commentId) const = 0;
};

} // namespace PddBy
