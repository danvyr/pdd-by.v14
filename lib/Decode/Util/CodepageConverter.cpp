#include "CodepageConverter.h"

#include "Shit.h"

#include <unicode/ucnv.h>

namespace PddBy
{

CodepageConverter::CodepageConverter(std::string const& fromCp, std::string const& toCp) :
    m_fromCp(fromCp),
    m_toCp(toCp)
{
    //
}

CodepageConverter::~CodepageConverter()
{
    //
}

std::string CodepageConverter::Convert(std::string const& text)
{
    std::string result(text.size() * 4, '\0');

    UErrorCode errCode = U_ZERO_ERROR;
    int32_t size = ucnv_convert(m_fromCp.c_str(), m_toCp.c_str(), &result[0], result.size(), text.c_str(), text.size(), &errCode);
    if (U_FAILURE(errCode))
    {
        throw Shit(u_errorName(errCode));
    }

    result.resize(size);
    return result;
}

} // namespace PddBy
