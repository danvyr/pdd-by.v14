#pragma once

#include <exception>
#include <string>

namespace boost
{

template<class CharT, class TraitsT, class AllocT>
class basic_format;

} // namespace boost

namespace PddBy
{

class Shit : public std::exception
{
public:
    Shit(std::string const& message) :
        m_message(message)
    {
        //
    }

    template<class CharT, class TraitsT, class AllocT>
    Shit(boost::basic_format<CharT, TraitsT, AllocT> const& formattedMessage) :
        m_message(formattedMessage.str())
    {
        //
    }

    virtual ~Shit() throw()
    {
        //
    }

    virtual char const* what() const throw()
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

} // namespace PddBy
