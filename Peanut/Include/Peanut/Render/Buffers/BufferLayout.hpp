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

enum class BufferLayoutAttributeUsage
{
    PerVertex,
    PerInstance
};

struct BufferLayoutElement
{
    BufferLayoutElement(uint32_t index, std::string name, BufferLayoutElementType type, uint32_t count);

    uint32_t index;
    std::string name;
    BufferLayoutElementType type;
    uint32_t count;
    uint32_t size;
    uint64_t offset;
};

class BufferLayout
{
public:
    // BufferLayout() = default;
    BufferLayout(BufferLayoutAttributeUsage usage, std::initializer_list<BufferLayoutElement> elements);

    const std::vector<BufferLayoutElement>& GetElements() const { return m_elements; }
    uint32_t GetStride() const { return m_stride; }
    BufferLayoutAttributeUsage GetUsage() const { return m_usage; }

    static std::shared_ptr<BufferLayout> Create(BufferLayoutAttributeUsage usage, std::initializer_list<BufferLayoutElement> elements);

private:
    std::vector<BufferLayoutElement> m_elements;
    uint32_t m_stride = 0;
    BufferLayoutAttributeUsage m_usage;

private:
    uint32_t GetTypeSize(BufferLayoutElementType type);
};

}

