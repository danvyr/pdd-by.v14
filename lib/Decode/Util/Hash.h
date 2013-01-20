#pragma once

#include "Buffer.h"

#include <string>

namespace PddBy
{

class Path;

class Hash
{
public:
    static Buffer RipeMd256FromString(std::string const& text);
    static Buffer Md5FromFile(Path const& path);

    static std::string ToHexString(Buffer const& buffer);
};

} // namespace PddBy
