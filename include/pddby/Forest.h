#pragma once

#include "IOak.h"

#include <memory>
#include <string>

namespace PddBy
{

typedef std::auto_ptr<IOak> IOakPtr;

class Forest
{
public:
    static IOakPtr CreateOak(std::string const& rootPath);
};

} // namespace PddBy
