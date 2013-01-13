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

namespace Magic
{

PddBy::Buffer const BpftFileHeader = { 'B', 'P', 'F', 'T' };
PddBy::Buffer const BpftcamFileHeader = { 'B', 'P', 'F', 'T', 'C', 'A', 'M' };

std::string const Oak14 = "npv05qcItAGPJUU3cIzHJvAPkSQFBxUqGrAfUmfJJxWr0=";

} // namespace Magic

namespace
{

std::string ConvertFromCp1251ToUtf8(char const* text, std::size_t length)
{
    std::string result(length * 3, '\0');

    UErrorCode errCode = U_ZERO_ERROR;
    std::int32_t size = ucnv_convert("utf-8", "cp1251", &result[0], result.size(), text, length, &errCode);
    if (U_FAILURE(errCode))
    {
        return std::string("<") + u_errorName(errCode) + ">";
    }

    result.resize(size);

    return result;
}

} // anonymous namespace

BOOST_AUTO_TEST_SUITE(FilterTestSuite)

BOOST_AUTO_TEST_CASE(BpftImageFilterTest)
{
    //return;

    using namespace PddBy;

    {
        std::string imageName = "1_001";

        std::unique_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftImageFilter(imageName + ".bpf", 0x91de));
        filter.reset(new HeaderCheckingFilter(std::move(filter), Magic::BpftFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(std::move(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s("../.test/" + imageName + ".png", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }

    // v14:
    //   * global magic = 0xc08bebf5

    {
        std::string imageName = "ABHDB";

        std::unique_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftImageFilter(imageName + ".bpf", static_cast<std::uint16_t>(0xc08bebf5) ^ 0x1234, 0x14));
        filter.reset(new HeaderCheckingFilter(std::move(filter), Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(std::move(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s("../.test/" + imageName + ".png", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }

    {
        std::string imageName = "H010100";

        std::unique_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftImageFilter(imageName + ".bpf", static_cast<std::uint16_t>(0xc08bebf5) ^ 0x1234, 0x14));
        filter.reset(new HeaderCheckingFilter(std::move(filter), Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(std::move(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s("../.test/" + imageName + ".png", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }
}

BOOST_AUTO_TEST_CASE(BpftcamTrashCartPrngImageFilterTest)
{
    //return;

    using namespace PddBy;

    {
        std::string imageName = "1_002";

        std::unique_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new BpftcamImageFilter(imageName + ".bpf", 0x04b5, BpftcamImageFilter::TrashCartPrng));
        filter.reset(new HeaderCheckingFilter(std::move(filter), Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(std::move(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s("../.test/" + imageName + ".png", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }
}

BOOST_AUTO_TEST_CASE(UglyPngImageFilterTest)
{
    //return;

    using namespace PddBy;

    {
        std::string imageName = "33617664";

        std::unique_ptr<FileReadStream> stream(new FileReadStream());
        BOOST_REQUIRE_NO_THROW(stream->Open(Path("../.test/" + imageName + ".bpf")));

        IFilterPtr filter(new UglyPngImageFilter(imageName));
        filter.reset(new TripleDesFilter(std::move(filter), Hash::RipeMd256FromString(imageName + Magic::Oak14)));
        filter.reset(new HeaderCheckingFilter(std::move(filter), Magic::BpftcamFileHeader));

        IReadStreamPtr imageStream;
        BOOST_REQUIRE_NO_THROW(imageStream = filter->Apply(std::move(stream)));

        Buffer buffer;
        BOOST_REQUIRE_NO_THROW(imageStream->ReadToEnd(buffer));

        BOOST_REQUIRE_EQUAL(std::string(reinterpret_cast<char const*>(&buffer[0]), 4), "\x89PNG");

        std::ofstream s("../.test/" + imageName + ".png", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        s.write(reinterpret_cast<char const*>(&buffer[0]), buffer.size());
    }
}

BOOST_AUTO_TEST_SUITE_END()
