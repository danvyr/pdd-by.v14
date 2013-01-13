#include "FileReadStream.h"

#include "Decode/Stream/StreamShit.h"
#include "Decode/Util/Path.h"

#include <boost/format.hpp>

namespace PddBy
{

FileReadStream::FileReadStream()
{
    //
}

FileReadStream::FileReadStream(Path const& path)
{
    Open(path);
}

FileReadStream::~FileReadStream()
{
    Close();
}

void FileReadStream::Open(Path const& path)
{
    try
    {
        m_stream.clear();
        m_stream.exceptions(std::ios_base::failbit);
        m_stream.open(path.ToString().c_str(), std::ios_base::in | std::ios_base::binary);
    }
    catch (std::ios_base::failure const&)
    {
        throw StreamShit(boost::format("Unable to open file for reading: %s") % path.ToString());
    }
}

void FileReadStream::Close()
{
    m_stream.close();
}

std::size_t FileReadStream::GetPosition()
{
    std::streampos const result = m_stream.tellg();
    if (result == -1)
    {
        throw StreamShit("Error getting stream position");
    }

    return result;
}

void FileReadStream::SetPosition(std::size_t offset)
{
    try
    {
        m_stream.clear();
        m_stream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
        m_stream.seekg(offset);
    }
    catch (std::ios_base::failure const& e)
    {
        throw StreamShit(boost::format("Error setting stream position: %s") % e.what());
    }
}

void FileReadStream::Read(Buffer& buffer, std::size_t size)
{
    buffer.resize(size);
    Read(&buffer[0], size);
}

void FileReadStream::Read(void* buffer, std::size_t size)
{
    if (size == 0)
    {
        return;
    }

    try
    {
        m_stream.clear();
        m_stream.exceptions(std::ios_base::badbit);
        m_stream.read(reinterpret_cast<char*>(buffer), size);
    }
    catch (std::ios_base::failure const& e)
    {
        throw StreamShit(boost::format("Error while reading from stream: %s") % e.what());
    }

    if (static_cast<std::size_t>(m_stream.gcount()) != size)
    {
        throw EndOfStreamShit("Premature end of file");
    }
}

void FileReadStream::ReadToEnd(Buffer& buffer)
{
    buffer.clear();

    while (!m_stream.eof())
    {
        Buffer tempBuffer(64 * 1024);

        try
        {
            m_stream.clear();
            m_stream.exceptions(std::ios_base::badbit);
            m_stream.read(reinterpret_cast<char*>(&tempBuffer[0]), tempBuffer.size());
        }
        catch (std::ios_base::failure const& e)
        {
            throw StreamShit(boost::format("Error while reading from stream: %s") % e.what());
        }

        tempBuffer.resize(m_stream.gcount());
        std::copy(tempBuffer.begin(), tempBuffer.end(), std::back_inserter(buffer));
    }
}

} // namespace PddBy
