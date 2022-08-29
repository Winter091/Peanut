#include "Peanut/Render/Buffers/BufferLayout.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

namespace pn {

    BufferLayoutElement::BufferLayoutElement(uint32_t index, std::string name, BufferLayoutElementType type, uint32_t count)
        : Index(index)
        , Name(std::move(name))
        , Type(type)
        , Count(count)
    {
        PN_CORE_ASSERT(count >= 1 && count <= 4, "Buffer layout element: invalid count: {}", count);
    }

    BufferLayout::BufferLayout(std::initializer_list<BufferLayoutElement> elements)
    {
        PN_PROFILE_FUNCTION();

        PN_CORE_ASSERT(elements.size() > 0, "Buffer layout has no attributes");
    
        m_elements.reserve(elements.size());

        uint32_t i = 0;
        uint32_t currOffset = 0;

        for (auto& element : elements) {
            m_elements.emplace_back(element);
        
            m_elements[i].SetSize(m_elements[i].GetCount() * GetTypeSize(m_elements[i].GetType()));
            m_elements[i].SetOffset(currOffset);

            currOffset += m_elements[i].GetSize();
            ++i;
        }

        m_vertexSize = currOffset;
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
            case BufferLayoutElementType::Mat4:
                return 4u * (4u * 4u);
            default:
                break;
        }

        PN_CORE_ASSERT(false, "Unsupported BufferLayoutElementType: {}", static_cast<uint32_t>(type));
        return 0u;
    }

}
