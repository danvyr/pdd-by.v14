#include "lib/Decode/Filter/HeaderCheckingFilter.h"
#include "lib/Decode/Filter/Image/BpftcamImageFilter.h"
#include "lib/Decode/Filter/Image/BpftImageFilter.h"
#include "lib/Decode/Filter/Image/UglyPngImageFilter.h"
#include "lib/Decode/Filter/NoopFilter.h"
#include "lib/Decode/Filter/TripleDesFilter.h"
#include "lib/Decode/Filter/TripleDes24Filter.h"
#include "lib/Decode/Stream/FileReadStream.h"
#include "lib/Decode/Stream/MemoryReadStream.h"
#include "lib/Decode/Util/Hash.h"
#include "lib/Decode/Util/Path.h"

#include <boost/test/unit_test.hpp>

#include <unicode/ucnv.h>

#include <iomanip>

namespace
{

PddBy::Buffer BufferFromString(std::string const& s)
{
    PddBy::Buffer buffer;
    for (std::size_t i = 0; i < s.size(); i++)
    {
        buffer.push_back(static_cast<PddBy::Buffer::value_type>(s[i]));
    }

    return buffer;
}

} // anonymous namespace

namespace Magic
{

PddBy::Buffer const BpftFileHeader = BufferFromString("BPFT");
PddBy::Buffer const BpftcamFileHeader = BufferFromString("BPFTCAM");

std::string const Oak14 = "npv05qcItAGPJUU3cIzHJvAPkSQFBxUqGrAfUmfJJxWr0=";

} // namespace Magic

BOOST_AUTO_TEST_SUITE(FilterTestSuite)

BOOST_AUTO_TEST_CASE(BpftImageFilterTest)
{
    using namespace PddBy;

    {
        std::string imageName = "1_001";

        std::auto_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftImageFilter(imageName + ".bpf", 0x91de));
        filter.reset(new HeaderCheckingFilter(filter, Magic::BpftFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(IReadStreamPtr(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s(("../.test/" + imageName + ".png").c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }

    {
        std::string imageName = "ABHDB";

        std::auto_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftImageFilter(imageName + ".bpf", static_cast<uint16_t>(0xc08bebf5) ^ 0x1234, 0x14));
        filter.reset(new HeaderCheckingFilter(filter, Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(IReadStreamPtr(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s(("../.test/" + imageName + ".png").c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }

    {
        std::string imageName = "H010100";

        std::auto_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftImageFilter(imageName + ".bpf", static_cast<uint16_t>(0xc08bebf5) ^ 0x1234, 0x14));
        filter.reset(new HeaderCheckingFilter(filter, Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(IReadStreamPtr(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s(("../.test/" + imageName + ".png").c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }
}

BOOST_AUTO_TEST_CASE(BpftcamTrashCartPrngImageFilterTest)
{
    using namespace PddBy;

    {
        std::string imageName = "1_002";

        std::auto_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftcamImageFilter(imageName + ".bpf", 0x04b5, BpftcamImageFilter::TrashCartPrng));
        filter.reset(new HeaderCheckingFilter(filter, Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(IReadStreamPtr(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s(("../.test/" + imageName + ".png").c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }
}

BOOST_AUTO_TEST_CASE(UglyPngImageFilterTest)
{
    using namespace PddBy;

    {
        std::string imageName = "33617664";

        std::auto_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new UglyPngImageFilter(imageName));
        filter.reset(new TripleDesFilter(filter, Hash::RipeMd256FromString(imageName + Magic::Oak14)));
        filter.reset(new HeaderCheckingFilter(filter, Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(IReadStreamPtr(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s(("../.test/" + imageName + ".png").c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }
}

BOOST_AUTO_TEST_SUITE_END()
