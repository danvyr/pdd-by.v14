#pragma once

#include <string>

namespace PddBy
{

class CodepageConverter
{
public:
    CodepageConverter(std::string const& fromCp, std::string const& toCp);
    ~CodepageConverter();

    std::string Convert(std::string const& text);

private:
    std::string const m_fromCp;
    std::string const m_toCp;
};

} // namespace PddBy
