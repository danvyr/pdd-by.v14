#pragma once

#include <string>
#include <vector>

namespace PddBy
{

struct QuestionLeaf
{
    std::string Id;
    std::string TopicId;
    std::string ImageId;
    std::string CommentId;
    std::string Text;
    std::vector<std::string> Answers;
    std::size_t CorrectAnswerIndex;
    std::string Advice;
};

} // namespace PddBy
