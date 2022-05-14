#include "BufferLayout.hpp"

#include <Peanut/Core/Assert.hpp>

namespace pn {

BufferLayoutElement::BufferLayoutElement(
    BufferLayoutElementType type, uint32_t count, const std::string& name, bool isNormalized
)
    : m_name(name), 
    m_index(0), 
    m_size(0), 
    m_offset(0), 
    m_type(type), 
    m_count(count),
    m_isNormalized(isNormalized)
{
    PN_CORE_ASSERT(m_count >= 1 && m_count <= 4, "Buffer layout: invalid count of elements: {}", m_count);
}

BufferLayout::BufferLayout(std::initializer_list<BufferLayoutElement> elements)
{
    m_elements.reserve(elements.size());

    uint32_t i = 0;
    uint32_t currOffset = 0;

    for (auto& element : elements) {
        m_elements.emplace_back(element);
        
        m_elements[i].m_index = i;
        m_elements[i].m_size = m_elements[i].m_count * GetTypeSize(m_elements[i].m_type);
        m_elements[i].m_offset = currOffset;

        currOffset += m_elements[i].m_size;
        ++i;
    }

    m_stride = currOffset;
}

uint32_t BufferLayout::GetTypeSize(BufferLayoutElementType type)
{
    switch (type) {
        case BufferLayoutElementType::Int8:
        case BufferLayoutElementType::Uint8:
            return 8u;
        case BufferLayoutElementType::Int16:
        case BufferLayoutElementType::Uint16:
            return 16u;
        case BufferLayoutElementType::Int32:
        case BufferLayoutElementType::Uint32:
            return 32u;
        case BufferLayoutElementType::Float:
            return 32u;
        case BufferLayoutElementType::Double:
            return 64u;
        default:
            break;
    }

    PN_CORE_ASSERT(false, "Unsupported BufferLayoutElementType: {}", static_cast<uint32_t>(type));
    return 0u;
}

}