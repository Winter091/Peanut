#include "Peanut/Render/Buffers/BufferLayout.hpp"

#include <Peanut/Core/Assert.hpp>

namespace pn {

BufferLayoutElement::BufferLayoutElement(
    BufferLayoutElementType type, uint32_t count, std::string name, bool isNormalized
)
    : name(std::move(name)), 
    index(0), 
    size(0), 
    offset(0), 
    type(type), 
    count(count),
    isNormalized(isNormalized)
{
    PN_CORE_ASSERT(count >= 1 && count <= 4, "Buffer layout: invalid count of elements: {}", count);
}

BufferLayout::BufferLayout(std::initializer_list<BufferLayoutElement> elements)
{
    PN_CORE_ASSERT(elements.size() > 0, "Buffer layout is empty");
    
    m_elements.reserve(elements.size());

    uint32_t i = 0;
    uint32_t currOffset = 0;

    for (auto& element : elements) {
        m_elements.emplace_back(element);
        
        m_elements[i].index = i;
        m_elements[i].size = m_elements[i].count * GetTypeSize(m_elements[i].type);
        m_elements[i].offset = currOffset;

        currOffset += m_elements[i].size;
        ++i;
    }

    m_stride = currOffset;
}

std::shared_ptr<BufferLayout> BufferLayout::Create(std::initializer_list<BufferLayoutElement> elements)
{
    return std::make_shared<BufferLayout>(elements);
}

uint32_t BufferLayout::GetTypeSize(BufferLayoutElementType type)
{
    switch (type) {
        case BufferLayoutElementType::Int8:
        case BufferLayoutElementType::Uint8:
            return 1u;
        case BufferLayoutElementType::Int16:
        case BufferLayoutElementType::Uint16:
            return 2u;
        case BufferLayoutElementType::Int32:
        case BufferLayoutElementType::Uint32:
        case BufferLayoutElementType::Float:
            return 4u;
        case BufferLayoutElementType::Double:
            return 8u;
        default:
            break;
    }

    PN_CORE_ASSERT(false, "Unsupported BufferLayoutElementType: {}", static_cast<uint32_t>(type));
    return 0u;
}

}