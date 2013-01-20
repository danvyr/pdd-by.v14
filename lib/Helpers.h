#pragma once

//#define CXX_CAN_INHERIT_CTORS

#ifdef CXX_CAN_INHERIT_CTORS
    #define PDDBY_INHERIT_SHIT_CTORS(Name, BaseName) using BaseName::BaseName;
#else
    #define PDDBY_INHERIT_SHIT_CTORS(Name, BaseName) \
    Name(std::string const& message) : BaseName(message) {} \
    template<class CharT, class TraitsT, class AllocT> \
    Name(boost::basic_format<CharT, TraitsT, AllocT> const& formattedMessage) : BaseName(formattedMessage) {}
#endif
