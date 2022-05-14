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

class BufferLayoutElement
{
public:
    BufferLayoutElement(BufferLayoutElementType type, uint32_t count, 
                        const std::string& name, bool isNormalized = false);

private:
    std::string m_name;
    uint32_t m_index;
    uint32_t m_size;
    uint32_t m_offset;
    BufferLayoutElementType m_type;
    uint32_t m_count;
    bool m_isNormalized;

private:
    friend class BufferLayout;
};

class BufferLayout
{
public:
    BufferLayout(std::initializer_list<BufferLayoutElement> elements);

    const std::vector<BufferLayoutElement>& GetElements() const { return m_elements; }
    uint32_t GetStride() const { return m_stride; }

private:
    std::vector<BufferLayoutElement> m_elements;
    uint32_t m_stride;

private:
    uint32_t GetTypeSize(BufferLayoutElementType type);
};

}

#endif
