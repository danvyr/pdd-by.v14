#include "Oak14ImageLimb.h"

#include "Decode/Filter/HeaderCheckingFilter.h"
#include "Decode/Filter/Image/BpftImageFilter.h"
#include "Decode/Filter/Image/UglyPngImageFilter.h"
#include "Decode/Filter/TripleDesFilter.h"
#include "Decode/Stream/FileReadStream.h"
#include "Decode/Util/Hash.h"
#include "Leaf/ImageLeaf.h"
#include "Shit.h"

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

namespace PddBy
{

namespace Detail
{

namespace Magic
{

PddBy::Buffer const FileHeader = BufferFromString("BPFTCAM");

} // namespace Magic

void ParseImageId(std::string const& imageId, std::string& imageDirName, std::string& imageFilename)
{
    std::size_t const slashPos = imageId.find('/');
    if (slashPos == std::string::npos)
    {
        throw Shit("Image identifier \"" + imageId + "\" is not valid");
    }

    imageDirName = imageId.substr(0, slashPos);
    imageFilename = imageId.substr(slashPos + 1);
}

} // namespace Detail

Oak14ImageLimb::Oak14ImageLimb(Path const& rootPath, std::string const& magicString, uint32_t magicNumber) :
    m_rootPath(rootPath),
    m_magicString(magicString),
    m_magicNumber(magicNumber)
{
    //
}

Oak14ImageLimb::~Oak14ImageLimb()
{
    //
}

ImageLeaf Oak14ImageLimb::GetImage(std::string const& imageId) const
{
    std::string imageDirName;
    std::string imageName;
    Detail::ParseImageId(imageId, imageDirName, imageName);

    IFilterPtr filter;
    if (imageDirName == "images")
    {
        filter.reset(new UglyPngImageFilter(imageName));
        filter.reset(new TripleDesFilter(filter, Hash::RipeMd256FromString(imageName + m_magicString)));
        filter.reset(new HeaderCheckingFilter(filter, Detail::Magic::FileHeader));
    }
    else if (imageDirName == "signs")
    {
        filter.reset(new BpftImageFilter(imageName + ".bpf", static_cast<uint16_t>(m_magicNumber) ^ 0x1234, 0x14));
        filter.reset(new HeaderCheckingFilter(filter, Detail::Magic::FileHeader));
    }

    if (filter.get() == 0)
    {
        throw Shit("Not implemented");
    }

    IReadStreamPtr fileStream(new FileReadStream(Path(m_rootPath / imageDirName / (imageName + ".bpf"))));
    IReadStreamPtr imageStream = filter->Apply(fileStream);

    ImageLeaf imageLeaf;
    imageLeaf.Id = imageId;
    imageStream->ReadToEnd(imageLeaf.Data);
    return imageLeaf;
}

} // namespace PddBy
