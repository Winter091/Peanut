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
        Mat4,
    };

    class BufferLayoutElement
    {
    public:
        BufferLayoutElement(uint32_t index, std::string name, BufferLayoutElementType type, uint32_t count);

        uint32_t GetIndex() const { return Index; }
        const std::string& GetName() const { return Name; }
        BufferLayoutElementType GetType() const { return Type; }
        uint32_t GetCount() const { return Count; }

        uint32_t GetSize() const { return Size; }
        void SetSize(uint32_t size) { Size = size; }

        uint64_t GetOffset() const { return Offset; }
        void SetOffset(uint64_t offset) { Offset = offset; }

    private:
        uint32_t Index;
        std::string Name;
        BufferLayoutElementType Type;
        uint32_t Count;
        uint32_t Size = 0;
        uint64_t Offset = 0;
    };

    class BufferLayout
    {
    public:
        BufferLayout(std::initializer_list<BufferLayoutElement> elements);

        const std::vector<BufferLayoutElement>& GetElements() const { return m_elements; }
        uint32_t GetVertexSize() const { return m_vertexSize; }

        static std::shared_ptr<BufferLayout> Create(std::initializer_list<BufferLayoutElement> elements);

    private:
        std::vector<BufferLayoutElement> m_elements;
        uint32_t m_vertexSize = 0;

    private:
        uint32_t GetTypeSize(BufferLayoutElementType type);
    };

}

