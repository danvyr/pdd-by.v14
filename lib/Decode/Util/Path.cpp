#include "Path.h"

#include "pddby/Shit.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

namespace PddBy
{

Path::Path(std::string const& path) :
    m_path(path)
{
    //
}

std::string const& Path::ToString() const
{
    return m_path;
}

std::string Path::GetBaseName() const
{
    std::size_t const slashPos = m_path.rfind('/');
    if (slashPos == std::string::npos)
    {
        return m_path;
    }

    return m_path.substr(slashPos + 1);
}

Path Path::operator / (std::string const& name) const
{
    for (boost::filesystem::directory_iterator dirIt(m_path), end; dirIt != end; ++dirIt)
    {
        std::string const entryName = dirIt->path().filename().string();
        if (boost::algorithm::iequals(name, entryName))
        {
            return Path(m_path + '/' + entryName);
        }
    }

    throw Shit("File not found: " + m_path + "/" + name);
}

} // namespace PddBy
