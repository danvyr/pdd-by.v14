#pragma once

#include <string>

namespace PddBy
{

class Path
{
public:
    Path(std::string const& path);

    std::string const& ToString() const;
    std::string GetBaseName() const;

    Path operator / (std::string const& name) const;

private:
    std::string m_path;
};

} // namespace PddBy
