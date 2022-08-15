#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Buffers/BufferLayout.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>

#include <cstdint>

#include <d3d11.h>

namespace pn {

    inline D3D11_USAGE BufferMapAccessToDxCpuUsageFlags(BufferMapAccess access)
    {
        switch (access) {
            case BufferMapAccess::NoAccess:     return D3D11_USAGE_IMMUTABLE;
            case BufferMapAccess::Read:         return D3D11_USAGE_STAGING;
            case BufferMapAccess::Write:        return D3D11_USAGE_DYNAMIC;
            case BufferMapAccess::WriteDiscard: return D3D11_USAGE_DYNAMIC;
            case BufferMapAccess::ReadWrite:    return D3D11_USAGE_STAGING;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
        return D3D11_USAGE_DEFAULT;
    }

    inline uint32_t BufferMapAccessToDxCpuAccessFlags(BufferMapAccess access)
    {
        switch (access) {
            case BufferMapAccess::NoAccess:     return 0u;
            case BufferMapAccess::Read:         return D3D11_CPU_ACCESS_READ;
            case BufferMapAccess::Write:        return D3D11_CPU_ACCESS_WRITE;
            case BufferMapAccess::WriteDiscard: return D3D11_CPU_ACCESS_WRITE;
            case BufferMapAccess::ReadWrite:    return D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
        return 0;
    }

    inline D3D11_MAP BufferMapAccessToDxMap(BufferMapAccess access)
    {
        PN_CORE_ASSERT(access != BufferMapAccess::NoAccess, "Unable to map buffer that don't support mapping");

        switch (access) {
            case BufferMapAccess::Read:         return D3D11_MAP_READ;
            case BufferMapAccess::Write:        return D3D11_MAP_WRITE;
            case BufferMapAccess::WriteDiscard: return D3D11_MAP_WRITE_DISCARD;
            case BufferMapAccess::ReadWrite:    return D3D11_MAP_READ_WRITE;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferMapAccess enum value");
        return D3D11_MAP_READ;
    }

    inline DXGI_FORMAT AttributeTypeToDxFormat(BufferLayoutElementType type, uint32_t count)
    {
        auto PrintAssertionError = [](BufferLayoutElementType type, uint32_t count) {
            PN_CORE_ASSERT(false, "Incorrect combination of type ({}) and count ({}) for dx11 attribute format",
                static_cast<uint32_t>(type), static_cast<uint32_t>(count));
        };

        switch (type) {
            case BufferLayoutElementType::Int8:
            {
                switch (count) {
                case 1:     return DXGI_FORMAT_R8_SINT;
                case 2:     return DXGI_FORMAT_R8G8_SINT;
                case 4:     return DXGI_FORMAT_R8G8B8A8_SINT;
                default:    PrintAssertionError(type, count); return DXGI_FORMAT_UNKNOWN;
                }
            }
            case BufferLayoutElementType::Uint8:
            {
                switch (count) {
                case 1:     return DXGI_FORMAT_R8_UINT;
                case 2:     return DXGI_FORMAT_R8G8_UINT;
                case 4:     return DXGI_FORMAT_R8G8B8A8_UINT;
                default:    PrintAssertionError(type, count); return DXGI_FORMAT_UNKNOWN;
                }
            }
            case BufferLayoutElementType::Int16:
            {
                switch (count) {
                case 1:     return DXGI_FORMAT_R16_SINT;
                case 2:     return DXGI_FORMAT_R16G16_SINT;
                case 4:     return DXGI_FORMAT_R16G16B16A16_SINT;
                default:    PrintAssertionError(type, count); return DXGI_FORMAT_UNKNOWN;
                }
            }
            case BufferLayoutElementType::Uint16:
            {
                switch (count) {
                case 1:     return DXGI_FORMAT_R16_UINT;
                case 2:     return DXGI_FORMAT_R16G16_UINT;
                case 4:     return DXGI_FORMAT_R16G16B16A16_UINT;
                default:    PrintAssertionError(type, count); return DXGI_FORMAT_UNKNOWN;
                }
            }
            case BufferLayoutElementType::Int32:
            {
                switch (count) {
                case 1:     return DXGI_FORMAT_R32_SINT;
                case 2:     return DXGI_FORMAT_R32G32_SINT;
                case 3:     return DXGI_FORMAT_R32G32B32_SINT;
                case 4:     return DXGI_FORMAT_R32G32B32A32_SINT;
                default:    PrintAssertionError(type, count); return DXGI_FORMAT_UNKNOWN;
                }
            }
            case BufferLayoutElementType::Uint32:
            {
                switch (count) {
                case 1:     return DXGI_FORMAT_R32_UINT;
                case 2:     return DXGI_FORMAT_R32G32_UINT;
                case 3:     return DXGI_FORMAT_R32G32B32_UINT;
                case 4:     return DXGI_FORMAT_R32G32B32A32_UINT;
                default:    PrintAssertionError(type, count); return DXGI_FORMAT_UNKNOWN;
                }
            }
            case BufferLayoutElementType::Float:
            {
                switch (count) {
                case 1:     return DXGI_FORMAT_R32_FLOAT;
                case 2:     return DXGI_FORMAT_R32G32_FLOAT;
                case 3:     return DXGI_FORMAT_R32G32B32_FLOAT;
                case 4:     return DXGI_FORMAT_R32G32B32A32_FLOAT;
                default:    PrintAssertionError(type, count); return DXGI_FORMAT_UNKNOWN;
                }
            }
            default: break;
        }

        PrintAssertionError(type, count); 
        return DXGI_FORMAT_UNKNOWN;
    }

    inline D3D11_INPUT_CLASSIFICATION BufferLayoutAttributeUsageToDx11InputSlotClass(BufferLayoutAttributeUsage usage)
    {
        switch (usage) {
            case BufferLayoutAttributeUsage::PerVertex:     return D3D11_INPUT_PER_VERTEX_DATA;
            case BufferLayoutAttributeUsage::PerInstance:   return D3D11_INPUT_PER_INSTANCE_DATA;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferLayoutAttributeUsage enum value");
        return D3D11_INPUT_PER_VERTEX_DATA;
    }

    inline uint32_t BufferLayoutAttributeUsageToDx11InstanceDataStepRate(BufferLayoutAttributeUsage usage)
    {
        switch (usage) {
            case BufferLayoutAttributeUsage::PerVertex:     return 0u;
            case BufferLayoutAttributeUsage::PerInstance:   return 1u;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown BufferLayoutAttributeUsage enum value");
        return 0u;
    }

    inline DXGI_FORMAT IndexBufferFormatToDx11Format(IndexBufferDataFormat format)
    {
        switch (format) {
            case IndexBufferDataFormat::Uint16:     return DXGI_FORMAT_R16_UINT;
            case IndexBufferDataFormat::Uint32:     return DXGI_FORMAT_R32_UINT;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown IndexBufferDataFormat enum value");
        return DXGI_FORMAT_UNKNOWN;
    }

    inline uint32_t GetIndexBufferDataFormatSize(IndexBufferDataFormat format)
    {
        switch (format) {
            case IndexBufferDataFormat::Uint16: return 2u;
            case IndexBufferDataFormat::Uint32: return 4u;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown IndexBufferDataFormat enum value: {}", static_cast<uint32_t>(format));
        return 0u;
    }

}

