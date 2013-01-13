#pragma once

#include "Decode/IReadStream.h"

#include <fstream>

namespace PddBy
{

class Path;

class FileReadStream : public IRandomAccessReadStream
{
public:
    FileReadStream();
    FileReadStream(Path const& path);
    virtual ~FileReadStream();

    void Open(Path const& path);
    void Close();

    // IRandomAccessReadStream implementation
    virtual std::size_t GetPosition();
    virtual void SetPosition(std::size_t offset);

    // IReadStream implementation
    virtual void Read(Buffer& buffer, std::size_t size);
    virtual void Read(void* buffer, std::size_t size);
    virtual void ReadToEnd(Buffer& buffer);

private:
    std::ifstream m_stream;
};

} // namespace PddBy
