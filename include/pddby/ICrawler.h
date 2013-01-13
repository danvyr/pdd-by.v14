#pragma once

namespace PddBy
{

template<typename T>
class ICrawler
{
public:
    virtual ~ICrawler() {}

    virtual bool GetNext(T& something) = 0;
};

} // namespace PddBy
