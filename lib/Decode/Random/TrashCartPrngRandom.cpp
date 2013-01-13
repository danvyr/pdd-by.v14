#include "TrashCartPrngRandom.h"

#include <algorithm>
#include <cstring>

namespace
{

std::uint32_t RotateLeft(std::uint32_t value, std::uint8_t shift)
{
    return (value << shift) | (value >> (sizeof(std::uint32_t) * 8 - shift));
}

std::uint32_t RotateRight(std::uint32_t value, std::uint8_t shift)
{
    return (value >> shift) | (value << (sizeof(std::uint32_t) * 8 - shift));
}

} // anonymous namespace

namespace PddBy
{

namespace Magic
{

std::size_t const KeySizeBits = 256;
std::size_t const KeySizeBytes = KeySizeBits / 8; // == 32
std::size_t const KeySize = KeySizeBytes / 4; // == 8
std::size_t const SubKeySize = KeySize / 2; // == 4

} // namespace Const

class TrashCartPrngRandom::Impl
{
public:
    typedef std::uint32_t KeyType[Magic::KeySize];
    typedef std::uint32_t SubKeyType[Magic::SubKeySize];

public:
    Impl();

    void SetKey(KeyType const& key);

    void Crypt(KeyStreamType& v);

private:
    void MakeTransformBox();

private:
    std::uint32_t m_idx1Null;
    std::uint32_t m_idx2Null;
    std::uint32_t m_idx3Null;
    std::uint32_t m_idxBox1[Magic::SubKeySize];
    std::uint32_t m_idxBox2[Magic::SubKeySize];
    std::uint32_t m_rotBox[Magic::KeySize];
    KeyType m_key;
    SubKeyType m_subKey1;
    SubKeyType m_subKey2;
};

TrashCartPrngRandom::Impl::Impl()
{
    //
}

void TrashCartPrngRandom::Impl::SetKey(KeyType const& key)
{
    std::memcpy(m_key, key, sizeof(m_key));

    MakeTransformBox();

    std::memcpy(m_subKey1, m_key, sizeof(m_subKey1));
    std::memcpy(m_subKey2, &m_key[Magic::SubKeySize], sizeof(m_subKey2));
}

void TrashCartPrngRandom::Impl::Crypt(KeyStreamType& v)
{
    // NOTE: operators precedence played a trick with author in all the expressions of type "x + (x << a) ^ (x >> b)";
    //       later versions have braces added around xor, while in this early one addition is executed first

    for (std::size_t n = 0; n < Magic::SubKeySize; n++)
    {
        std::uint32_t idx1 = m_idxBox1[n];
        std::uint32_t idx2 = m_idxBox2[n];

        for (std::size_t i = 0; i < Magic::SubKeySize; i++)
        {
            std::uint32_t const k1 = m_subKey1[idx1];
            std::uint32_t const k2 = m_subKey1[idx1 + 1];
            std::uint32_t const k3 = m_subKey2[idx2];
            std::uint32_t const k4 = m_subKey2[idx2 + 1];

            std::uint32_t const rot1 = m_rotBox[idx2 + idx1];
            std::uint32_t const rot2 = m_rotBox[idx2 + idx1 + 1];

            if (idx1 == m_idx1Null)
            {
                v[0] += v[3] + RotateRight((v[1] + (v[1] << 6)) ^ (v[1] >> 9), rot1) + k1;
            }
            else
            {
                v[0] += v[3] + RotateRight(((v[1] + (v[1] << 6)) ^ (v[1] >> 9)) + k1, rot1);
            }

            if (idx2 == m_idx2Null)
            {
                v[1] += v[2] + RotateRight((v[0] + (v[0] << 6)) ^ (v[0] >> 9), rot2) + k2;
            }
            else
            {
                v[1] += v[2] + RotateRight(((v[0] + (v[0] << 6)) ^ (v[0] >> 9)) + k2, rot2);
            }

            if (i == m_idx3Null)
            {
                v[2] += ((v[0] + (v[0] << 4)) ^ (v[0] >> 5)) + k3;
                v[3] += ((v[1] + (v[1] << 4)) ^ (v[1] >> 5)) + k4;
            }
            else
            {
                v[2] ^= ((v[0] + (v[0] << 4)) ^ (v[0] >> 5)) + k3;
                v[3] ^= ((v[1] + (v[1] << 4)) ^ (v[1] >> 5)) + k4;
            }

            std::swap(v[0], v[2]);
            std::swap(v[1], v[3]);

            idx1 = (idx1 + 1) % 3;
            idx2 = (idx2 + 1) % 3;
        }
    }
}

void TrashCartPrngRandom::Impl::MakeTransformBox()
{
    // NOTE: operators precedence played a trick with author in all the expressions of type "x + (x << a) ^ (x >> b)";
    //       later versions have braces added around xor, while in this early one addition is executed first

    m_idx1Null = 0;
    m_idx2Null = 0;
    m_idx3Null = 0;

    for (std::size_t i = 0; i < Magic::SubKeySize; i++)
    {
        m_idxBox1[i] = RotateRight((m_key[i] + (m_key[i] << 4)) ^ (m_key[i] >> 5), (i % 31) + 1) % (Magic::SubKeySize - 1);
        m_idx1Null = ((m_idx1Null * (m_idx1Null + 1)) >> 1) + m_key[i];
    }

    for (std::size_t i = Magic::SubKeySize; i < Magic::KeySize; i++)
    {
        m_idxBox2[Magic::KeySize - 1 - i] = RotateLeft((m_key[i] + (m_key[i] << 4)) ^ (m_key[i] >> 5), (i % 31) + 1) %
            (Magic::SubKeySize - 1);
        m_idx2Null = ((m_idx2Null * (m_idx2Null + 1)) >> 1) + m_key[i];
    }

    for (std::size_t i = 0; i < Magic::KeySize; i++)
    {
        m_rotBox[i] = RotateLeft(m_key[i] ^ (i + 1), (Magic::KeySize - 1 - i) % 31 + 1) % 31 + 1;
        m_idx3Null += m_key[i] * (i + 1);
    }

    m_idx1Null = (m_idx1Null ^ (m_idx1Null >> 16)) % Magic::SubKeySize;
    m_idx2Null = (m_idx2Null ^ (m_idx2Null >> 16)) % Magic::SubKeySize;
    m_idx3Null = (m_idx3Null ^ (m_idx3Null >> 16)) % Magic::SubKeySize;
}

TrashCartPrngRandom::TrashCartPrngRandom(std::uint32_t seed) :
    m_impl(new Impl())
{
    union
    {
        struct __attribute__((packed))
        {
            Impl::KeyType Key;
            KeyStreamType KeyStream;
        } Typed;

        std::uint32_t Raw[(sizeof(Impl::KeyType) + sizeof(KeyStreamType)) / sizeof(std::uint32_t)];
    } key;

    key.Raw[0] = seed;
    for (std::size_t i = 1; i < sizeof(key.Raw) / sizeof(*key.Raw); i++)
    {
        key.Raw[i] = key.Raw[i - 1] * 69069 + 1;
    }

    m_impl->SetKey(key.Typed.Key);
    std::memcpy(m_keyStream, key.Typed.KeyStream, sizeof(m_keyStream));
}

TrashCartPrngRandom::~TrashCartPrngRandom()
{
    //
}

std::uint8_t TrashCartPrngRandom::GetNext()
{
    m_impl->Crypt(m_keyStream);

    return m_keyStream[0];
}

} // namespace PddBy
