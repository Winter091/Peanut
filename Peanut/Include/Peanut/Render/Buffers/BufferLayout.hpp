#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

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
    BufferLayoutElement(uint32_t index, BufferLayoutElementType type, uint32_t count, 
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
    // BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferLayoutElement> elements);

    const std::vector<BufferLayoutElement>& GetElements() const { return m_elements; }
    uint32_t GetStride() const { return m_stride; }

    static std::shared_ptr<BufferLayout> Create(std::initializer_list<BufferLayoutElement> elements);

private:
    std::vector<BufferLayoutElement> m_elements;
    uint32_t m_stride = 0;

private:
    uint32_t GetTypeSize(BufferLayoutElementType type);
};

}

