#pragma once

#include <memory>
#include <string>

namespace PddBy
{

class IOak;
typedef std::unique_ptr<IOak> IOakPtr;

class Forest
{
public:
    static IOakPtr CreateOak(std::string const& rootPath);
};

} // namespace PddBy
