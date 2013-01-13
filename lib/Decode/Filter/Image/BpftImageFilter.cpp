#include "BpftImageFilter.h"

#include "BpftHelper.h"

#include "Decode/Random/DelphiRandom.h"
#include "Decode/Stream/BaseFilteringReadStream.h"

//#include "pddby/Shit.h"

namespace PddBy
{

//namespace Magic
//{
//
//std::string const FileHeader = "BPFT";
//
//} // namespace Magic

namespace Detail
{

template<typename RandomT>
class RandomXoringReadStream : public BaseFilteringReadStream
{
public:
    RandomXoringReadStream(IReadStreamPtr stream, std::unique_ptr<RandomT> random) :
        BaseFilteringReadStream(std::move(stream)),
        m_random(std::move(random))
    {
        //
    }

protected:
    virtual void ApplyFilter(std::uint8_t* buffer, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i++)
        {
            buffer[i] ^= m_random->GetNext(255);
        }
    }

private:
    std::unique_ptr<RandomT> m_random;
};

} // namespace Detail

BpftImageFilter::BpftImageFilter(std::string const& imageName, std::uint16_t magicNumber, std::uint16_t magicSeed) :
    m_imageName(imageName),
    m_magicNumber(magicNumber),
    m_magicSeed(magicSeed)
{
    //
}

IReadStreamPtr BpftImageFilter::Apply(IReadStreamPtr stream)
{
//    Buffer tempBuffer;
//    stream->Read(tempBuffer, Magic::FileHeader.size());
//
//    if (std::string(reinterpret_cast<char const*>(&tempBuffer[0]), Magic::FileHeader.size()) != Magic::FileHeader)
//    {
//        throw Shit("Image is not a valid BPFT image");
//    }

    std::unique_ptr<DelphiRandom> random(new DelphiRandom(BpftHelper::ImageNameToRandSeed(m_imageName, m_magicNumber) ^
        m_magicSeed));
    return IReadStreamPtr(new Detail::RandomXoringReadStream<DelphiRandom>(std::move(stream), std::move(random)));
}

} // namespace PddBy
