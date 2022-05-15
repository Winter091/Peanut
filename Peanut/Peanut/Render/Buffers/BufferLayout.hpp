#ifndef __BUFFERLAYOUT_H__
#define __BUFFERLAYOUT_H__

#include <cstdint>
#include <string>
#include <vector>

namespace pn {

enum class BufferLayoutElementType : uint32_t 
{
    Int8 = 0,
    Uint8,
    Int16,
    Uint16,
    Int32,
    Uint32,
    Float,
    Double,
};

struct BufferLayoutElement
{
    BufferLayoutElement(BufferLayoutElementType type, uint32_t count, 
                        std::string name, bool isNormalized = false);

    std::string name;
    uint32_t index;
    uint32_t size;
    uint64_t offset;
    BufferLayoutElementType type;
    uint32_t count;
    bool isNormalized;
};

class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferLayoutElement> elements);
    BufferLayout(const BufferLayout&) = default;

    const std::vector<BufferLayoutElement>& GetElements() const { return m_elements; }
    uint32_t GetStride() const { return m_stride; }

private:
    std::vector<BufferLayoutElement> m_elements;
    uint32_t m_stride = 0;

private:
    uint32_t GetTypeSize(BufferLayoutElementType type);
};

}

#endif
