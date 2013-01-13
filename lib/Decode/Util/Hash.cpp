#include "Hash.h"

#include "Path.h"

#include <boost/algorithm/hex.hpp>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/md5.h>
#include <cryptopp/ripemd.h>

namespace PddBy
{

Buffer Hash::RipeMd256FromString(std::string const& text)
{
    CryptoPP::RIPEMD256 hasher;
    Buffer hash(CryptoPP::RIPEMD256::DIGESTSIZE);
    CryptoPP::StringSource(text, true, new CryptoPP::HashFilter(hasher, new CryptoPP::ArraySink(&hash[0], hash.size())));
    return hash;
}

Buffer Hash::Md5FromFile(Path const& path)
{
    CryptoPP::Weak::MD5 hasher;
    Buffer hash(CryptoPP::Weak::MD5::DIGESTSIZE);
    CryptoPP::FileSource(path.ToString().c_str(), true, new CryptoPP::HashFilter(hasher, new CryptoPP::ArraySink(&hash[0],
        hash.size())));
    return hash;
}

std::string Hash::ToHexString(Buffer const& buffer)
{
    std::string result;
    boost::algorithm::hex(buffer.begin(), buffer.end(), std::back_inserter(result));
    return result;
}

} // namespace PddBy
